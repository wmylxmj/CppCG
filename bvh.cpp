//
// Created by wmy on 2023/11/14.
//

#include "bvh.h"

enum {
    P1, P2, P3, MEAN, MIN, MAX
};

enum {
    X, Y, Z
};

void write_cache(Triangle* triangles, const Matrix& vertex_coordinates) {
    size_t ntris = _msize(triangles) / sizeof(Triangle);
    for (size_t i = 0; i < ntris; i++) {
        triangles[i].cache = new float[6 * 3];
        Matrix cache(triangles[i].cache, 6, 3);
        memcpy(cache[P1], vertex_coordinates[triangles[i].vertices[0]], 3 * sizeof(float));
        memcpy(cache[P2], vertex_coordinates[triangles[i].vertices[1]], 3 * sizeof(float));
        memcpy(cache[P3], vertex_coordinates[triangles[i].vertices[2]], 3 * sizeof(float));
        cache[MEAN][X] = cblas_ssum(3, cache[P1] + X, 3) / 3;
        cache[MEAN][Y] = cblas_ssum(3, cache[P1] + Y, 3) / 3;
        cache[MEAN][Z] = cblas_ssum(3, cache[P1] + Z, 3) / 3;
        cache[MIN][X] = std::min(std::min(cache[P1][X], cache[P2][X]), cache[P3][X]);
        cache[MIN][Y] = std::min(std::min(cache[P1][Y], cache[P2][Y]), cache[P3][Y]);
        cache[MIN][Z] = std::min(std::min(cache[P1][Z], cache[P2][Z]), cache[P3][Z]);
        cache[MAX][X] = std::max(std::max(cache[P1][X], cache[P2][X]), cache[P3][X]);
        cache[MAX][Y] = std::max(std::max(cache[P1][Y], cache[P2][Y]), cache[P3][Y]);
        cache[MAX][Z] = std::max(std::max(cache[P1][Z], cache[P2][Z]), cache[P3][Z]);
    }
}

void clear_cache(Triangle* triangles) {
    size_t ntris = _msize(triangles) / sizeof(Triangle);
    for (size_t i = 0; i < ntris; i++) {
        delete[] triangles[i].cache;
        triangles[i].cache = nullptr;
    }
}

bool cmpx(const Triangle& triA, const Triangle& triB) {
    Matrix A(triA.cache, 6, 3);
    Matrix B(triB.cache, 6, 3);
    return A[MEAN][X] < B[MEAN][X];
}

bool cmpy(const Triangle& triA, const Triangle& triB) {
    Matrix A(triA.cache, 6, 3);
    Matrix B(triB.cache, 6, 3);
    return A[MEAN][Y] < B[MEAN][Y];
}

bool cmpz(const Triangle& triA, const Triangle& triB) {
    Matrix A(triA.cache, 6, 3);
    Matrix B(triB.cache, 6, 3);
    return A[MEAN][Z] < B[MEAN][Z];
}

void buildBVHCore(BVHNode* node, Triangle* triangles, size_t left, size_t right, std::vector<Coordinate>& AABB, size_t max_ntris) {
    Coordinate AA = {FLT_MAX, FLT_MAX, FLT_MAX};
    Coordinate BB = {FLT_MIN, FLT_MIN, FLT_MIN};
    // get AABB
    for (size_t i = left; i < right; i++) {
        Matrix triangle(triangles[i].cache, 6, 3);
        AA.x = std::min(AA.x, triangle[MIN][X]);
        AA.y = std::min(AA.y, triangle[MIN][Y]);
        AA.z = std::min(AA.z, triangle[MIN][Z]);
        BB.x = std::max(BB.x, triangle[MAX][X]);
        BB.y = std::max(BB.y, triangle[MAX][Y]);
        BB.z = std::max(BB.z, triangle[MAX][Z]);
    }
    // update bounding boxes
    node->aa = AABB.size();
    node->bb = AABB.size() + 1;
    AABB.push_back(AA);
    AABB.push_back(BB);
    // insufficient number of triangles, return leaf nodes
    if ((right - left) <= max_ntris) {
        node->ntris = right - left;
        node->index = left;
        return;
    }
    // build the tree
    float len_x = BB.x - AA.x;
    float len_y = BB.y - AA.y;
    float len_z = BB.z - AA.z;
    if (len_x >= len_y && len_x >= len_z) {
        std::sort(triangles+left, triangles+right, cmpx);
    }
    else if (len_y >= len_x && len_y >= len_z) {
        std::sort(triangles+left, triangles+right, cmpy);
    }
    else if (len_z >= len_x && len_z >= len_y) {
        std::sort(triangles+left, triangles+right, cmpz);
    }
    size_t mid = (right + left) / 2;
    node->left_child = new BVHNode();
    buildBVHCore(node->left_child, triangles, left, mid, AABB, max_ntris);
    node->right_child = new BVHNode();
    buildBVHCore(node->right_child, triangles, mid, right, AABB, max_ntris);
    return;
}

void BVH::build(Triangle* triangles, const Matrix& vertex_coordinates, size_t max_ntris) {
    this->destroy();
    write_cache(triangles, vertex_coordinates);
    size_t ntris = _msize(triangles) / sizeof(Triangle);
    this->tree = new BVHNode();
    buildBVHCore(this->tree, triangles, 0, ntris, this->AABB, max_ntris);
    clear_cache(triangles);
}

void destroyBVHCore(BVHNode* node) {
    if (node == nullptr) return;
    if (node->left_child) {
        destroyBVHCore(node->left_child);
        node->left_child = nullptr;
    }
    if (node->right_child) {
        destroyBVHCore(node->right_child);
        node->right_child = nullptr;
    }
    delete node;
}

void BVH::destroy() {
    destroyBVHCore(this->tree);
    this->tree = nullptr;
    this->AABB.shrink_to_fit();
}

BVH::~BVH() {
    this->destroy();
}

bool hitAABB(const Ray& ray, const Vector3& AA, const Vector3& BB) {
    Vector3 t_AA = (AA - ray.origin) / ray.direction;
    Vector3 t_BB = (BB - ray.origin) / ray.direction;
    Vector3 t_before = minimum(t_AA, t_BB);
    Vector3 t_after = maximum(t_AA, t_BB);
    float t_in = max(t_before);
    float t_out = min(t_after);
    if (t_in > t_out || t_out < 0) return false;
    return true;
}

float hitTriangleCore(const Ray& ray, const Vector3& P1, const Vector3& P2, const Vector3& P3) {
    Vector3 n = normalize(cross(P2-P1, P3-P2));
    float inner = dot(n, ray.direction);
    // if parallel
    if (inner == 0) return FLT_MAX;
    // the triangle normal vector is opposite to the ray direction
    if (inner > 0) n = n * -1;
    float t = (dot(n, P1) - dot(ray.origin, n)) / dot(ray.direction, n);
    // if the triangle is behind the ray
    if (t < 0) return FLT_MAX;
    Vector3 P = ray.origin + ray.direction * t;
    // determine whether the intersection point is in the triangle
    Vector3 C1 = cross(P2-P1, P-P1);
    Vector3 C2 = cross(P3-P2, P-P2);
    Vector3 C3 = cross(P1-P3, P-P3);
    if (dot(C1, n) > 0 && dot(C2, n) > 0 && dot(C3, n) > 0) return t;
    if (dot(C1, n) < 0 && dot(C2, n) < 0 && dot(C3, n) < 0) return t;
    return FLT_MAX;
}

float hitTriangle(const Ray& ray, Triangle* triangle, const Matrix& vertex_coordinates) {
    Vector3 P1(vertex_coordinates[triangle->vertices[0]], 1);
    Vector3 P2(vertex_coordinates[triangle->vertices[1]], 1);
    Vector3 P3(vertex_coordinates[triangle->vertices[2]], 1);
    return hitTriangleCore(ray, P1, P2, P3);
}

HitResult hitTriangleArray(const Ray& ray, Triangle* triangles, size_t index, size_t ntris, const Matrix& vertex_coordinates) {
    HitResult res;
    for (size_t i = index; i < (index + ntris); i++) {
        float t = hitTriangle(ray, triangles + i, vertex_coordinates);
        if (t < res.t) {
            res.t = t;
            res.triangle = triangles + i;
        }
    }
    return res;
}

HitResult hitBVHCore(const Ray& ray, Triangle* triangles, const Matrix& vertex_coordinates, const Matrix& AABB, BVHNode* node) {
    if (!node) {
        HitResult res;
        return res;
    }
    if (node->ntris > 0) {
        return hitTriangleArray(ray, triangles, node->index, node->ntris, vertex_coordinates);
    }
    HitResult res_left, res_right;
    if (node->left_child) {
        Vector3 AA(AABB[node->left_child->aa], 1);
        Vector3 BB(AABB[node->left_child->bb], 1);
        if (hitAABB(ray, AA, BB)) {
            res_left = hitBVHCore(ray, triangles, vertex_coordinates, AABB, node->left_child);
        }
    }
    if (node->right_child) {
        Vector3 AA(AABB[node->right_child->aa], 1);
        Vector3 BB(AABB[node->right_child->bb], 1);
        if (hitAABB(ray, AA, BB)) {
            res_right = hitBVHCore(ray, triangles, vertex_coordinates, AABB, node->right_child);
        }
    }
    return res_left.t < res_right.t ? res_left : res_right;
}

HitResult BVH::hit(const Ray& ray, Triangle *triangles, const Matrix &vertex_coordinates, const Matrix& AABB_coordinates) {
    return hitBVHCore(ray, triangles, vertex_coordinates, AABB_coordinates, this->tree);
}