//
// Created by wmy on 2023/11/19.
//

#ifndef CPPCG_CAMERA_H
#define CPPCG_CAMERA_H

#include "vec3calc.h"
#include "matcalc.h"
#include "transform.h"
#include "bvh.h"
#include <cmath>
#include <random>
#include <iostream>

typedef struct {
    size_t width;
    size_t height;
} Resolution;

class Camera {
private:
    Vector3 _eye_position = Vector3(0, 0, 0);
    Vector3 _camera_angle = Vector3(0, 0, 0);
    float _fov = M_PI / 3;
    Resolution _resolution = {1280, 780};
    float _aspect = (float)_resolution.width / (float)_resolution.height;
    Matrix _coordinate_system = identity_matrix(3);
    float _top = tanf(_fov / 2.0f);
    float _bottom = - _top;
    float _right = _top * _aspect;
    float _left = - _right;

public:
    void rotate_xy(float dtheta_x, float dtheta_y);
    void set_resolution(const size_t (&resolution)[2]);
    void set_fov(float fov);
    void set_eye_position(float x, float y, float z);
    Ray get_ray(float u, float v);
};

#endif //CPPCG_CAMERA_H
