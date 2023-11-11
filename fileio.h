//
// Created by wmy on 2023/11/9.
//

#ifndef CPPCG_FILEIO_H
#define CPPCG_FILEIO_H

#include <sstream>
#include <fstream>
#include <iostream>
#include "matrix.h"

typedef struct {
    unsigned int vertices[3];
    unsigned int textures[3];
    unsigned int normals[3];
} Triangle;

class OBJFile
{
public:
    Matrix<float> vertex_coordinates;
    Matrix<float> texture_coordinates;
    Matrix<float> normal_coordinates;
    Triangle* triangles = nullptr;
    void read(const std::string& fp);
    void clear() {
        vertex_coordinates.clear();
        texture_coordinates.clear();
        normal_coordinates.clear();
        delete[] triangles;
    }
    ~OBJFile() {
        clear();
    }
};

#endif //CPPCG_FILEIO_H
