#include <iostream>
#include "svpng.inc"
#include <omp.h>
#include <cstring>
#include <vector>

#include "fileio.h"
#include "matcalc.h"
#include "bvh.h"
#include "vec3calc.h"
#include "camera.h"

Matrix get_homogeneous_coordinates(const Matrix& coordinates) {
    Matrix homogeneous_coordinates(coordinates.shape(0), 4);
    cblas_scopy((blasint)coordinates.shape(0), coordinates[0] + 0, 3, homogeneous_coordinates[0] + 0, 4);
    cblas_scopy((blasint)coordinates.shape(0), coordinates[0] + 1, 3, homogeneous_coordinates[0] + 1, 4);
    cblas_scopy((blasint)coordinates.shape(0), coordinates[0] + 2, 3, homogeneous_coordinates[0] + 2, 4);
    for (size_t i = 0; i < homogeneous_coordinates.shape(0); i++) homogeneous_coordinates[i][3] = 1;
    return homogeneous_coordinates;
}

void imsave(const float* IMAGE, size_t WIDTH, size_t HEIGHT, const char* FILEPATH) {
    auto* UINT8_IMAGE = new unsigned char[HEIGHT * WIDTH * 3];
    for (size_t i = 0; i < HEIGHT * WIDTH * 3; i++) {
        UINT8_IMAGE[i] = (unsigned char) std::clamp(IMAGE[i] * 255, 0.0f, 255.0f);
    }
    FILE* fp;
    fopen_s(&fp, FILEPATH, "wb");
    svpng(fp, WIDTH, HEIGHT, UINT8_IMAGE, 0);
    delete[] UINT8_IMAGE;
}

int main() {
    OBJFile o;
    o.read("G:/CppCG/test.obj");
    BVH b;
    b.build(o.triangles, o.vertex_coordinates);
    // get get_homogeneous coordinates
    Matrix vertex_coordinates = get_homogeneous_coordinates(o.vertex_coordinates);
    Matrix AABB_link((float*)b.AABB.data(), (size_t)b.AABB.size(), 3);
    Matrix AABB_coordinates = get_homogeneous_coordinates(AABB_link);
    // transform
    float xmean = cblas_ssum((blasint)vertex_coordinates.shape(0), vertex_coordinates[0] + 0, (blasint)vertex_coordinates.shape(1)) / (float)vertex_coordinates.shape(0);
    float ymean = cblas_ssum((blasint)vertex_coordinates.shape(0), vertex_coordinates[0] + 1, (blasint)vertex_coordinates.shape(1)) / (float)vertex_coordinates.shape(0);
    float zmin = FLT_MAX;
    for (size_t i = 0; i < vertex_coordinates.shape(0); i++) zmin = vertex_coordinates[i][2] < zmin ? vertex_coordinates[i][2] : zmin;
    Matrix M = Mtranslation(-xmean, -ymean, -zmin);
    M = matmul(Mrotate(Vector3(1, 0, 0), -M_PI/2), M, false, false, false);
    M = matmul(Mtranslation(0, 0, -60), M, false, false, false);
    vertex_coordinates = matmul(M, vertex_coordinates, false, true, true);
    AABB_coordinates = matmul(M, AABB_coordinates, false, true, true);
    // camera
    Camera c;
    const size_t WIDTH = 256;
    const size_t HEIGHT = 256;
    c.set_resolution({WIDTH, HEIGHT});
    c.set_eye_position(0, 25, 0);
    // image
    auto* image = new float[HEIGHT * WIDTH * 3];
    memset(image, 0, sizeof(float) * HEIGHT * WIDTH * 3);
    // compute
#pragma omp parallel for
    for (size_t i = 0; i < HEIGHT; i++) {
        std::cout << i << std::endl;
#pragma omp parallel for
        for (size_t j = 0; j < WIDTH; j++) {
            float v = (float)i + 0.5f;
            float u = (float)j + 0.5f;
            Ray ray = c.get_ray(u, v);
            HitResult res = b.hit(ray, o.triangles, vertex_coordinates, AABB_coordinates);
            if (res.t < FLT_MAX) {
                Vector3 P1(vertex_coordinates[res.triangle->vertices[0]], 1);
                Vector3 P2(vertex_coordinates[res.triangle->vertices[1]], 1);
                Vector3 P3(vertex_coordinates[res.triangle->vertices[2]], 1);
                Vector3 normal = normalize(cross(P2-P1, P3-P2));
                float gray = fabsf(dot(normal, Vector3(0, 0, -1)));
                float* pix_ptr = image + i * (WIDTH * 3) + j * 3;
                for (size_t k = 0; k < 3; k++) {
                    pix_ptr[k] = gray;
                }
            }
        }
    }
    imsave(image, WIDTH, HEIGHT, "test.png");
    // clear heap memory
    delete[] image;
    std::cout << "end" << std::endl;

    return 0;
}

