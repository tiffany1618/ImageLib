#ifndef PHOTOEDITOR_UTIL_H
#define PHOTOEDITOR_UTIL_H

#include <functional>

#include "Image.h"
#include "Matrix.h"

// Colorspace transformation constants
const double RGB_TO_XYZ_SCAL =  1.0 / 0.17697;
const double RGB_TO_XYZ_MAT[3][3] = {
        {0.49, 0.31, 0.20},
        {0.17697, 0.81240, 0.01063},
        {0.0, 0.01,0.99}
};
const double XYZ_TO_RGB_SCAL = 0.17697;
const double XYZ_TO_RGB_MAT[3][3] = {
        {2.36461, -0.89654, -0.46807},
        {-0.51516, 1.42640, 0.08875},
        {0.00520, -0.01440, 1.00920}
};

// Image helper functions
typedef std::function<uint8_t(uint8_t, uint8_t, uint8_t)> func_point_op;
typedef std::function<uint8_t(int)> func_lut;

void create_lookup_table(uint8_t *lut, func_lut f);
Image point_op_lut(const Image &input, const uint8_t *lut);

Image colorspace_change(const Image &input, const Matrix<double> &mat);

#endif //PHOTOEDITOR_UTIL_H
