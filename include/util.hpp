#ifndef PHOTOEDITOR_UTIL_H
#define PHOTOEDITOR_UTIL_H

#include <string>
#include <functional>
#include <map>

#include "Image.h"
#include "../Matrix.h"

// Colorspace transformation constants
const double LIN_RGB_GAMMA = 2.2;
const double sRGB_TO_XYZ_MAT[3][3] = {
        {0.4124564, 0.3575761, 0.1804375},
        {0.2126729, 0.7151522, 0.0721750},
        {0.0193339, 0.1191920, 0.9503041}
};
const double XYZ_TO_sRGB_MAT[3][3] = {
        {3.2404542, -1.5371385, -0.4985314},
        {-0.9692660, 1.8760108, 0.0415560},
        {0.0556434, -0.2040259, 1.0572252}
};
const double CIERGB_TO_XYZ_MAT[3][3] = {
        {0.49, 0.31, 0.20},
        {0.17697, 0.81240, 0.01063},
        {0.0, 0.01,0.99}
};
const double XYZ_TO_CIERGB_MAT[3][3] = {
        {2.36461, -0.89654, -0.46807},
        {-0.51516, 1.42640, 0.08875},
        {0.00520, -0.01440, 1.00920}
};

// Image helper functions
void generate_xyz_tristimulus_vals(const std::string &ref_white, double &x_n, double &y_n, double &z_n);
double xyz_to_lab_func(double num);
double lab_to_xyz_func(double num);
void generate_histogram_percentiles(const Image<double> &input, std::map<double, double> &percentiles);

// Template image helper functions definitions
template<typename T>
uint8_t cast_uint8_t(T num);
template<typename T>
void create_lookup_table(T *lut, std::function<T(int)> f);
template<typename T>
Image<T> point_op_lut(const Image<uint8_t> &input, const T *lut);
template<typename T, typename S>
Image<S> point_op(const Image<T> &input, std::function<S(T)> f);

template<typename T>
Image<T> linear_colorspace_change(const Image<T> &input, const Matrix<double> &mat);

// Template image helper functions implementations
template<typename T>
uint8_t cast_uint8_t(T num) {
    num = round(num);

    if (num < 0) num = 0;
    if (num > UINT8_MAX) num = UINT8_MAX;

    return static_cast<uint8_t>(num);
}

template<typename T>
void create_lookup_table(T *lut, std::function<T(int)> f) {
    for (int i = 0; i < 256; i++) {
        lut[i] = f(i);
    }
}

template<typename T>
Image<T> point_op_lut(const Image<uint8_t> &input, const T *lut) {
    Image<T> output(input.get_width(), input.get_height(), input.get_channels());

    uint8_t *i;
    T *j;
    for (i = input.get_data(), j = output.get_data(); i != input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        *j = lut[*i];
        *(j + 1) = lut[*(i + 1)];
        *(j + 2) = lut[*(i + 2)];

        // Copy over transparency channel if it exists
        if (input.get_channels() == 4) {
            *(j + 3) = static_cast<T>(*(i + 3));
        }
    }

    return output;
}

template<typename T, typename S>
Image<S> point_op(const Image<T> &input, std::function<S(T)> f) {
    Image<S> output(input.get_width(), input.get_height(), input.get_channels());

    T *i;
    S *j;
    for (i = input.get_data(), j = output.get_data(); i != input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        *j = f(*i);
        *(j + 1) = f(*(i + 1));
        *(j + 2) = f(*(i + 2));

        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
}

template<typename T>
Image<T> linear_colorspace_change(const Image<T> &input, const Matrix<double> &mat) {
    Image<T> output(input.get_width(), input.get_height(), input.get_channels());
    Matrix<double> orig(3, 1);
    Matrix<double> temp(3, 1);

    for (T *i = input.get_data(), *j = output.get_data(); i != input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        orig.get_mat()[0][0] = *i;
        orig.get_mat()[1][0] = *(i + 1);
        orig.get_mat()[2][0] = *(i + 2);

        temp = mat * orig;

        *j = temp.get_mat()[0][0];
        *(j + 1) = temp.get_mat()[1][0];
        *(j + 2) = temp.get_mat()[2][0];

        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
}

#endif //PHOTOEDITOR_UTIL_H
