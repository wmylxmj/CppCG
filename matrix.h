//
// Created by wmy on 2023/11/10.
//

#ifndef CPPCG_MATRIX_H
#define CPPCG_MATRIX_H

#include <iostream>

class Matrix {
private:
    float* _vals;
    size_t _m, _n;

public:
    [[nodiscard]] size_t shape(size_t i) const;
    Matrix();
    Matrix(size_t m, size_t n);
    void rebuild(size_t m, size_t n);
    ~Matrix();
    void clear();
    float* operator[](size_t i) const;
    Matrix& operator=(const Matrix& matrix);
    void print();
};

#endif //CPPCG_MATRIX_H
