//
// Created by wmy on 2023/11/16.
//

#include "vec3calc.h"

Vector3 operator+(const Vector3& A, const Vector3& B) {
    Vector3 C;
    memcpy(C.data(), B.data(), 3 * sizeof(float));
    cblas_saxpby(3, 1, A.data(), A.inc(), 1, C.data(), C.inc());
    return C;
}

Vector3 operator-(const Vector3& A, const Vector3& B) {
    Vector3 C;
    memcpy(C.data(), B.data(), 3 * sizeof(float));
    cblas_saxpby(3, 1, A.data(), A.inc(), -1, C.data(), C.inc());
    return C;
}

Vector3 operator*(const Vector3& A, const Vector3& B) {
    Vector3 C;
    for (int i = 0; i < 3; i++) {
        C[i] = A[i] * B[i];
    }
    return C;
}

Vector3 operator/(const Vector3& A, const Vector3& B) {
    Vector3 C;
    for (int i = 0; i < 3; i++) {
        C[i] = A[i] / B[i];
    }
    return C;
}

float dot(const Vector3& A, const Vector3& B) {
    return cblas_sdot(3, A.data(), A.inc(), B.data(), B.inc());
}

Vector3 cross(const Vector3& A, const Vector3& B) {
    Vector3 C;
    C[0] = A[1] * B[2] + A[2] * B[1];
    C[1] = A[2] * B[0] + A[0] * B[2];
    C[2] = A[0] * B[1] + A[1] * B[0];
    return C;
}

Vector3 minimum(const Vector3& A, const Vector3& B) {
    Vector3 C;
    C[0] = A[0] < B[0] ? A[0] : B[0];
    C[1] = A[1] < B[1] ? A[1] : B[1];
    C[2] = A[2] < B[2] ? A[2] : B[2];
    return C;
}

Vector3 maximum(const Vector3& A, const Vector3& B) {
    Vector3 C;
    C[0] = A[0] > B[0] ? A[0] : B[0];
    C[1] = A[1] > B[1] ? A[1] : B[1];
    C[2] = A[2] > B[2] ? A[2] : B[2];
    return C;
}

float max(const Vector3& A) {
    return std::max(std::max(A[0], A[1]), A[2]);
}

float min(const Vector3& A) {
    return std::min(std::min(A[0], A[1]), A[2]);
}

Vector3 normalize(const Vector3& A) {
    Vector3 C;
    float norm = sqrtf(powf(A[0], 2) + powf(A[1], 2) + powf(A[2], 2));
    if (norm == 0) {
        C = {0, 0, 0};
        return C;
    }
    C[0] /= norm;
    C[1] /= norm;
    C[2] /= norm;
    return C;
}

