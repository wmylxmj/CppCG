//
// Created by wmy on 2023/11/9.
//

#include "fileio.h"

bool startswith(const std::string& str, const std::string& head) {
    if (str.size() < head.size()) {
        return false;
    }
    return str.substr(0, head.size()) == head;
}

template <typename dtype>
float str2num(const std::string& str) {
    std::istringstream iss(str);
    dtype num;
    iss >> num;
    return num;
}

// get vertex position from line
template <size_t dim, typename dtype>
void get_coordinates(const std::string& str, float* const arr) {
    int pos = 0, n = 0, i = 0;
    auto valid = [] (char buffer) -> bool {
        return (buffer>=48 && buffer<=57) || \
        buffer == '+' || buffer == '-' || \
        buffer == '.' || buffer == 'e';
    };
    for (char buffer : str) {
        if (valid(buffer)) {
            n++;
        }
        else {
            if (n > 0) {
                *(arr + i++) = str2num<dtype>(str.substr(pos, n));
                if (i == dim) return;
            }
            pos += n + 1;
            n = 0;
        }
    }
    if (n > 0) {
        *(arr + i) = str2num<dtype>(str.substr(pos, n));
    }
}

unsigned int count_triangles(const std::string& str) {
    unsigned int count = 0;
    int pos = 0, n = 0;
    auto valid = [] (char buffer) -> bool {
        return (buffer >= 48 && buffer <= 57) || buffer == '/';
    };
    for (char buffer : str) {
        if (valid(buffer)) {
            n++;
        }
        else {
            if (n > 0) {
                count++;
            }
            pos += n + 1;
            n = 0;
        }
    }
    if (n > 0) {
        count++;
    }
    return count - 2;
}

unsigned int get_triangles(const std::string& str, Triangle* const triangles) {
    unsigned int inner = 0, outer = 0;
    int pos = 0; int n = 0;
    auto set = \
    [] (Triangle* const triangles, unsigned int inner, unsigned int outer, unsigned int num) -> void {
        if (inner == 0) {
            if (outer > 2) {
                (triangles + outer - 2)->vertices[0] = (triangles + outer - 3)->vertices[0];
                (triangles + outer - 2)->vertices[1] = (triangles + outer - 3)->vertices[2];
                (triangles + outer - 2)->vertices[2] = num;
            }
            else triangles->vertices[outer] = num;
        }
        if (inner == 1) {
            if (outer > 2) {
                (triangles + outer - 2)->textures[0] = (triangles + outer - 3)->textures[0];
                (triangles + outer - 2)->textures[1] = (triangles + outer - 3)->textures[2];
                (triangles + outer - 2)->textures[2] = num;
            }
            else triangles->textures[outer] = num;
        }
        if (inner == 2) {
            if (outer > 2) {
                (triangles + outer - 2)->normals[0] = (triangles + outer - 3)->normals[0];
                (triangles + outer - 2)->normals[1] = (triangles + outer - 3)->normals[2];
                (triangles + outer - 2)->normals[2] = num;
            } else triangles->normals[outer] = num;
        }
    };
    for (char buffer : str) {
        if (buffer >= 48 && buffer <= 57) {
            n++;
        }
        else if (buffer == '/') {
            if (n > 0) {
                set(triangles, inner, outer, (unsigned int) (str2num<int>(str.substr(pos, n)) - 1));
            }
            pos += n + 1;
            n = 0;
            inner++;
        }
        else {
            if (n > 0) {
                set(triangles, inner, outer, (unsigned int) (str2num<int>(str.substr(pos, n)) - 1));
                inner = 0;
                outer++;
            }
            pos += n + 1;
            n = 0;
        }
    }
    if (n > 0) {
        set(triangles, inner, outer, (unsigned int) (str2num<int>(str.substr(pos, n)) - 1));
        outer++;
    }
    return outer - 2;
}

void OBJFile::read(const std::string& fp) {
    this->clear();
    unsigned int nv = 0, nvt = 0, nvn = 0, ntris = 0;
    std::ifstream f(fp);
    std::string line;
    // count first
    while (std::getline(f, line)) {
        if (startswith(line, "v ")) nv++;
        else if (startswith(line, "vt")) nvt++;
        else if (startswith(line, "vn")) nvn++;
        else if (startswith(line, "f ")) {
            ntris += count_triangles(line);
        }
    }
    f.clear();
    f.seekg(0, std::ios::beg);
    // load data
    unsigned int iv = 0, ivt = 0, ivn = 0, itris = 0;
    this->vertex_coordinates.rebuild(nv, 3);
    this->texture_coordinates.rebuild(nvt, 2);
    this->normal_coordinates.rebuild(nvn, 3);
    this->triangles = new Triangle[ntris];
    while (std::getline(f, line)) {
        if (startswith(line, "v ")) {
            get_coordinates<3, float>(line, this->vertex_coordinates[iv++]);
        }
        else if (startswith(line, "vt")) {
            get_coordinates<2, float>(line, this->texture_coordinates[ivt++]);
        }
        else if (startswith(line, "vn")) {
            get_coordinates<3, float>(line, this->normal_coordinates[ivn++]);
        }
        else if (startswith(line, "f ")) {
            itris += get_triangles(line, this->triangles + itris);
        }
    }
    f.close();
}
