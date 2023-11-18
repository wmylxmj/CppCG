//
// Created by wmy on 2023/11/18.
//

#ifndef CPPCG_TRANSFORM_H
#define CPPCG_TRANSFORM_H

#include "matcalc.h"
#include "vec3calc.h"
#include <cmath>

extern Matrix Mrodrigues(const Vector3& rotAxis, float rotAngle);
extern Matrix Mrotate(const Vector3& rotAxis, float rotAngle);
extern Matrix Mtranslation(float x, float y, float z);
extern Matrix Mscale(float x, float y, float z);


#endif //CPPCG_TRANSFORM_H
