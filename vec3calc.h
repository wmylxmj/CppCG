//
// Created by wmy on 2023/11/16.
//

#ifndef CPPCG_VEC3CALC_H
#define CPPCG_VEC3CALC_H

#include "vector3.h"
#include <cblas.h>
#include <cmath>

extern Vector3 operator+(const Vector3& A, const Vector3& B);
extern Vector3 operator-(const Vector3& A, const Vector3& B);
extern Vector3 operator*(const Vector3& A, const Vector3& B);
extern Vector3 operator/(const Vector3& A, const Vector3& B);

extern float dot(const Vector3& A, const Vector3& B);
extern Vector3 cross(const Vector3& A, const Vector3& B);
extern Vector3 minimum(const Vector3& A, const Vector3& B);
extern Vector3 maximum(const Vector3& A, const Vector3& B);

extern float max(const Vector3& A);
extern float min(const Vector3& A);

extern Vector3 normalize(const Vector3& A);

#endif //CPPCG_VEC3CALC_H
