//
// Created by wmy on 2023/11/10.
//

#include "matrix.h"

size_t Matrix::shape(size_t i) const {
    return i ? _n : _m;
}

Matrix::Matrix() : _m(0), _n(0), _vals(nullptr) {}

Matrix::Matrix(size_t m, size_t n) : _m(m), _n(n) {
    if (m * n) _vals = new float[m * n];
    else {
        _m = 0; _n = 0;
        _vals = nullptr;
    }
}

void Matrix::rebuild(size_t m, size_t n) {
    if (m * n) {
        delete[] _vals;
        _m = m; _n = n;
        _vals = new float[m * n];
    }
    else clear();
}

Matrix::~Matrix() {
    delete[] _vals;
}

void Matrix::clear() {
    delete[] _vals;
    _m = 0; _n = 0;
    _vals = nullptr;
}

float* Matrix::operator[](size_t i) const {
    return _vals + i * _n;
}

Matrix& Matrix::operator=(const Matrix& matrix) {
    if (this == &matrix) return *this;
    rebuild(matrix.shape(0), matrix.shape(1));
    if (_m * _n) {
        memcpy(_vals, matrix[0], _m * _n * sizeof(float));
    }
    return *this;
}

void Matrix::print() {
    for (int i = 0; i < _m; i++) {
        for (int j = 0; j < _n; j++) {
            std::cout << _vals[i * _n + j] << " ";
        }
        std::cout << std::endl;
    }
}

