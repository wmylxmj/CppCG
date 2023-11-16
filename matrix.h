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
    bool _link;

public:
    [[nodiscard]] size_t shape(size_t i) const;
    Matrix();
    Matrix(size_t m, size_t n);
    void rebuild(size_t m, size_t n);
    Matrix(Matrix&& matrix) noexcept;
    ~Matrix();
    void clear();
    float* operator[](size_t i) const;
    Matrix& operator=(Matrix&& matrix) noexcept;
    void print();
    Matrix(float* ptr, size_t m, size_t n);
    void link(float* ptr, size_t m, size_t n);
};

#endif //CPPCG_MATRIX_H