//
// Created by wmy on 2023/11/18.
//

#include "transform.h"

// 3D
Matrix Mrodrigues(const Vector3& rotAxis, float rotAngle) {
    float kx = rotAxis[0], ky = rotAxis[1], kz = rotAxis[2];
    Matrix k(3, 1);
    k[0][0] = kx; k[1][0] = ky; k[2][0] = kz;
    Matrix Mparallel = matmul(k, k, false, true, false);
    Matrix Mvertical = identity_matrix(3) - Mparallel;
    Matrix Mw(3, 3);
    Mw[0][0] = 0; Mw[0][1] = -kz; Mw[0][2] = ky;
    Mw[1][0] = kz; Mw[1][1] = 0; Mw[1][2] = -kx;
    Mw[2][0] = -ky; Mw[2][1] = kx; Mw[2][2] = 0;
    Matrix Mrot = cosf(rotAngle) * Mvertical + sinf(rotAngle) * Mw;
    return Mparallel + Mrot;
}

// 4D
Matrix Mrotate(const Vector3& rotAxis, float rotAngle) {
    Matrix Mrot = identity_matrix(4);
    Matrix rotCore = Mrodrigues(rotAxis, rotAngle);
    memcpy(Mrot[0] + 0, rotCore[0] + 0, 3 * sizeof(float));
    memcpy(Mrot[0] + 1, rotCore[0] + 1, 3 * sizeof(float));
    memcpy(Mrot[0] + 2, rotCore[0] + 2, 3 * sizeof(float));
    return Mrot;
}

Matrix Mtranslation(float x, float y, float z) {
    Matrix M = identity_matrix(4);
    M[0][3] = x;
    M[1][3] = y;
    M[2][3] = z;
    return M;
}

Matrix Mscale(float x, float y, float z) {
    Matrix M = identity_matrix(4);
    M[0][0] = x;
    M[1][1] = y;
    M[2][2] = z;
    return M;
}
