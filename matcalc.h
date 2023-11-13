//
// Created by wmy on 2023/11/13.
//

#ifndef CPPCG_MATCALC_H
#define CPPCG_MATCALC_H

#include "matrix.h"
#include <cstring>
#include <cblas.h>

extern Matrix identity_matrix(size_t size);
extern Matrix matmul(const Matrix& A, const Matrix& B, bool transA, bool transB, bool transC);

#endif //CPPCG_MATCALC_H
