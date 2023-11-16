//
// Created by wmy on 2023/11/16.
//

#include "vector3.h"

int Vector3::inc() const {
    return _inc;
}

float *Vector3::data() const {
    return _ptr;
}

Vector3::Vector3() = default;

Vector3::Vector3(const float (&arr)[3]) {
    _ptr = new float[3];
    _inc = 1;
    memcpy(_ptr, arr, 3 * sizeof(float));
    _link = false;
}

Vector3::Vector3(float x, float y, float z) {
    _ptr = new float[3];
    _inc = 1;
    _ptr[0] = x; _ptr[1] = y; _ptr[2] = z;
    _link = false;
}

Vector3::Vector3(float *ptr, int inc) {
    _ptr = ptr;
    _inc = inc;
    _link = true;
}

Vector3::Vector3(Vector3&& vec3) noexcept {
    _ptr = vec3._ptr;
    _inc = vec3._inc;
    _link = vec3._link;
    vec3._ptr = nullptr;
    vec3._inc = 0;
    vec3._link = false;
}

Vector3::~Vector3() {
    if (!_link) delete[] _ptr;
}

void Vector3::clear() {
    if (!_link) delete[] _ptr;
    _ptr = nullptr;
    _inc = 0;
    _link = false;
}

float& Vector3::operator[](int i) {
    return _ptr[i];
}

const float& Vector3::operator[](int i) const {
    return _ptr[i];
}

Vector3& Vector3::operator=(Vector3&& vec3) noexcept {
    if (this == &vec3) return *this;
    if (!_link) delete[] _ptr;
    _ptr = vec3._ptr;
    _inc = vec3._inc;
    _link = vec3._link;
    vec3._ptr = nullptr;
    vec3._inc = 0;
    vec3._link = false;
    return *this;
}

Vector3& Vector3::operator=(const float (&arr)[3]) {
    if (!_link) delete[] _ptr;
    _ptr = new float[3];
    _inc = 1;
    memcpy(_ptr, arr, 3 * sizeof(float));
    _link = false;
    return *this;
}

void Vector3::link(float *ptr, int inc) {
    if (!_link) delete[] _ptr;
    _ptr = ptr;
    _inc = inc;
    _link = true;
}

void Vector3::print() {
    for (int i = 0; i < 3; i++) {
        std::cout << _ptr[0] << " ";
    }
    std::cout << std::endl;
}