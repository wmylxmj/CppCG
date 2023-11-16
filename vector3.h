//
// Created by wmy on 2023/11/16.
//

#ifndef CPPCG_VECTOR3_H
#define CPPCG_VECTOR3_H

#include <cstring>
#include <iostream>

class Vector3 {
private:
    float* _ptr = nullptr;
    int _inc = 0;
    bool _link = false;

public:
    [[nodiscard]] int inc() const;
    [[nodiscard]] float* data() const;
    Vector3();
    explicit Vector3(const float (&arr)[3]);
    Vector3(float x, float y, float z);
    explicit Vector3(float*ptr, int inc = 1);
    Vector3(Vector3&& vec3) noexcept;
    ~Vector3();
    void clear();
    float& operator[](int i);
    const float& operator[](int i) const;
    Vector3& operator=(Vector3&& vec3) noexcept;
    Vector3& operator=(const float (&arr)[3]);
    void link(float* ptr, int inc);
    void print();
};

#endif //CPPCG_VECTOR3_H