//
// Created by wmy on 2023/11/14.
//

#ifndef CPPCG_BVH_H
#define CPPCG_BVH_H

#include "fileio.h"
#include <cblas.h>
#include <cfloat>
#include <algorithm>
#include <cmath>
#include <vector>
#include "vec3calc.h"

typedef struct {
    Vector3 origin;
    Vector3 direction;
} Ray;

struct HitResult {
    Triangle* triangle = nullptr;
    float t = FLT_MAX;
};

typedef struct {
    float x, y, z;
} Coordinate;

typedef struct BVHNode {
    BVHNode* left_child = nullptr;
    BVHNode* right_child = nullptr;
    size_t index = 0, ntris = 0;
    size_t aa = 0, bb = 0;
} BVHNode;

class BVH {
public:
    BVHNode *tree = nullptr;
    std::vector<Coordinate> AABB;
    void build(Triangle *triangles, const Matrix &vertex_coordinates, size_t max_ntris = 2);
    void destroy();
    ~BVH();
    HitResult
    hit(const Ray &ray, Triangle *triangles, const Matrix &vertex_coordinates, const Matrix &AABB_coordinates);
};

extern void write_cache(Triangle* triangles, const Matrix& vertex_coordinates);
extern void clear_cache(Triangle* triangles);

#endif //CPPCG_BVH_H
