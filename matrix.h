//
// Created by wmy on 2023/11/10.
//

#ifndef CPPCG_MATRIX_H
#define CPPCG_MATRIX_H

#include <iostream>
#include <cstring>
#include <cblas.h>

template <typename dtype>
class Matrix {
private:
    dtype* _data;
    unsigned int _m, _n;

public:
    [[nodiscard]] unsigned int shape(unsigned int i) const {
        return i ? _n : _m;
    }

    Matrix() : _m(0), _n(0), _data(nullptr) {}

    Matrix(unsigned int m, unsigned int n) : _m(m), _n(n) {
        if (m * n) _data = new dtype[m*n];
        else {
            _m = 0; _n = 0;
            _data = nullptr;
        }
    }

    void rebuild(unsigned int m, unsigned int n) {
        if (m * n) {
            delete[] _data;
            _m = m; _n = n;
            _data = new dtype[m*n];
        }
        else clear();
    }

    ~Matrix() {
        delete[] _data;
    }

    void clear() {
        delete[] _data;
        _m = 0; _n = 0;
        _data = nullptr;
    }

    dtype* operator[](unsigned int i) const {
        return _data + i * _n;
    }

    Matrix<dtype>& operator=(const Matrix<dtype>& matrix) {
        if (this == &matrix) return *this;
        rebuild(matrix.shape(0), matrix.shape(1));
        if (_m * _n) {
            memcpy(_data, matrix[0], _m*_n);
        }
        return *this;
    }

    void print() {
        for (int i = 0; i < _m; i++) {
            for (int j = 0; j < _n; j++) {
                std::cout << this->_data[i * _n + j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

template <typename dtype, unsigned int n>
class vec {
private:
    dtype _data[n];

public:
    constexpr unsigned int size() {
        return n;
    }

    dtype& operator[](unsigned int i) {
        return _data[i];
    }

    void print() {
        for (int i = 0; i < n; i++) {
            std::cout << _data[i] << " ";
        }
        std::cout << std::endl;
    }
};

template <typename dtype, unsigned int m, unsigned int n>
class mat {
private:
    dtype _data[m * n];

public:
    constexpr unsigned int shape(unsigned int i) {
        return i ? n : m;
    }

    dtype* operator[](unsigned int i) {
        return _data + i * n;
    }

    mat<dtype, m, n>& operator=(const mat<dtype, m, n> &matrix) {
        if (this == &matrix) return *this;
        memcpy(_data, matrix, m * n);
        return *this;
    }

    void print() {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << this->_data[i * n + j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

template <typename dtype, unsigned int size>
mat<dtype, size, size> get_identity_matrix() {
    mat<dtype, size, size> I;
    memset(I[0], 0, size * size * sizeof(dtype));
    for (int i = 0; i < size; i++) {
        I[i][i] = 1;
    }
    return I;
}

#endif //CPPCG_MATRIX_H
