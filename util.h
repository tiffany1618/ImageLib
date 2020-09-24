//
// Created by tiffany on 9/22/20.
//

#ifndef PHOTOEDITOR_UTIL_H
#define PHOTOEDITOR_UTIL_H

#include <functional>

#include "Image.h"

// RGB to XYZ transformation
const double RGB_TO_XYZ_SCAL =  1.0 / 0.17697;
const double RGB_TO_XYZ_MAT[3][3] = {
        {0.49, 0.31, 0.20},
        {0.17697, 0.181240, 0.01063},
        {0.0, 0.01,0.99}
};

// Image helper functions
typedef std::function<uint8_t(uint8_t, uint8_t, uint8_t)> func_point_op;
typedef std::function<uint8_t(int)> func_lut;

Image point_op(const Image &input, func_point_op f);
Image point_op_lut(const Image &input, const uint8_t *lut);
void create_lookup_table(uint8_t *lut, func_lut f);

#endif //PHOTOEDITOR_UTIL_H
