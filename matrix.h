//
// Created by wmy on 2023/11/10.
//

#ifndef CPPCG_MATRIX_H
#define CPPCG_MATRIX_H

#include <iostream>

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
        delete[] _data;
        _m = matrix.shape(0);
        _n = matrix.shape(1);
        _data = new dtype[_m*_n];
        memcpy(_data, matrix[0], _m*_n);
        return *this;
    }

    void print() {
        for (int i = 0; i < _m; i++) {
            for (int j = 0; j < _n; j++)
                std::cout << this->_data[i*_n + j] << " ";
            std::cout << std::endl;
        }
    }
};

#endif //CPPCG_MATRIX_H
