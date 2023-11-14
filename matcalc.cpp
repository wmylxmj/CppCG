//
// Created by wmy on 2023/11/13.
//

#include "matcalc.h"

Matrix identity_matrix(size_t size) {
    Matrix I(size, size);
    memset(I[0], 0, size * size * sizeof(float));
    for(int i = 0; i < size; i++) I[i][i] = 1;
    return I;
}

Matrix matmulCore(const Matrix& A, const Matrix& B, bool transA, bool transB) {
    size_t m = transA ? A.shape(1) : A.shape(0);
    size_t n = transB ? B.shape(0) : B.shape(1);
    size_t k = transB ? B.shape(1) : B.shape(0);
    Matrix C(m, n);
    CBLAS_TRANSPOSE TransA = transA ? CblasTrans : CblasConjNoTrans;
    CBLAS_TRANSPOSE TransB = transB ? CblasTrans : CblasConjNoTrans;
    int lda = (int) A.shape(1);
    int ldb = (int) B.shape(1);
    int ldc = (int) C.shape(1);
    cblas_sgemm(CblasRowMajor, TransA, TransB, (int)m, (int)n, (int)k, 1, A[0], lda, B[0], ldb, 0, C[0], ldc);
    return C;
}

Matrix matmul(const Matrix& A, const Matrix& B, bool transA, bool transB, bool transC) {
    if (transC) {
        return matmulCore(B, A, !transB, !transA);
    }
    return matmulCore(A, B, transA, transB);
}

Matrix operator+(const Matrix& A, const Matrix& B) {
    Matrix C(A.shape(0), A.shape(1));
    memcpy(C[0], B[0], A.shape(0) * A.shape(1) * sizeof(float));
    cblas_saxpby((blasint)(A.shape(0)*A.shape(1)), 1, A[0], 1, 1, C[0], 1);
    return C;
}

Matrix operator-(const Matrix& A, const Matrix& B) {
    Matrix C(A.shape(0), A.shape(1));
    memcpy(C[0], B[0], A.shape(0) * A.shape(1) * sizeof(float));
    cblas_saxpby((blasint)(A.shape(0)*A.shape(1)), 1, A[0], 1, -1, C[0], 1);
    return C;
}

Matrix operator*(float a, const Matrix& B) {
    Matrix C(B.shape(0), B.shape(1));
    cblas_saxpby((blasint)(B.shape(0)*B.shape(1)), a, B[0], 1, 0, C[0], 1);
    return C;
}