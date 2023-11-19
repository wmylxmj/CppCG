//
// Created by wmy on 2023/11/19.
//

#include "camera.h"

void Camera::rotate_xy(float dtheta_x, float dtheta_y) {
    float x = sinf(_camera_angle[0]);
    float z = - cosf(_camera_angle[0]);
    Matrix Mtheta_y = Mrodrigues(Vector3(z, 0, x), dtheta_y);
    Matrix Mtheta_x = Mrodrigues(Vector3(0, 1, 0), dtheta_x);
    Matrix Mrot = matmul(Mtheta_x, Mtheta_y, false, false, false);
    _coordinate_system = matmul(Mrot, _coordinate_system, false, true, true);
    _camera_angle[0] += dtheta_x;
    _camera_angle[1] += dtheta_y;
}

void Camera::set_resolution(const size_t (&resolution)[2]) {
    _resolution.width = resolution[0];
    _resolution.height = resolution[1];
    _aspect = (float)_resolution.width / (float)_resolution.height;
    _right = _top * _aspect;
    _left = - _right;
}

void Camera::set_fov(float fov) {
    _fov = fov;
    _top = tanf(_fov / 2.0f);
    _bottom = - _top;
    _right = _top * _aspect;
    _left = - _right;
}

void Camera::set_eye_position(float x, float y, float z) {
    _eye_position[0] = x;
    _eye_position[1] = y;
    _eye_position[2] = z;
}

Ray Camera::get_ray(float u, float v) {
    Ray ray;
    float dh = (_top - _bottom) / (float) _resolution.height;
    float dw = (_right - _left) / (float) _resolution.width;
    float hcomp = _top - v * dh;
    float wcomp = _left + u * dw;
    Vector3 x(_coordinate_system[0], 1);
    Vector3 y(_coordinate_system[1], 1);
    Vector3 z(_coordinate_system[2], 1);
    ray.direction = -1 * z + hcomp * y + wcomp * x;
    ray.origin[0] = _eye_position[0];
    ray.origin[1] = _eye_position[1];
    ray.origin[2] = _eye_position[2];
    return ray;
}