#include <iostream>
#include <cmath>

#include "core.h"
#include "util.h"
#include "Matrix.h"

Image<uint8_t> to_grayscale(const Image<uint8_t> &input) {
    int channels;
    if (input.get_channels() == 4) {
        channels = 2;
    } else {
        channels = 1;
    }

    Image<uint8_t> output(input.get_width(), input.get_height(), channels);

    for (uint8_t *i = input.get_data(), *j = output.get_data(); i != input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        *j = (*i + (*i + 1) + *(i + 2)) / 3.0;

        if (input.get_channels() == 4) {
            *(j + 1) = *(i + 3);
        }
    }

    return output;
}

Image<double> linearize_srgb(const Image<uint8_t> &input) {
    double lut[256];
    create_lookup_table<double>(lut, [] (int num) -> double {
        double val = static_cast<double>(num);
        if (val <= 10) {
            val /= 3294.6;
        } else {
            val = pow((val + 14.025) / 269.025, 2.4);
        }

        return val;
    });

    return point_op_lut<double>(input, lut);
}

Image<uint8_t> unlinearize_srgb(const Image<double> &input) {
    return point_op<double, uint8_t>(input, [] (double num) -> uint8_t {
        if (num <= 0.0031308) {
            num *= 3294.6;
        } else {
            num = 269.025 * pow(num, 1.0 /  2.4) - 14.025;
        }

        return static_cast<uint8_t>(num + 0.5);
    });
}

Image<double> srgb_to_xyz(const Image<double> &input) {
    Matrix<double> trans(3, 3);
    for (int i = 0; i < trans.get_rows(); i++) {
        for (int j = 0; j < trans.get_cols(); j++) {
            trans.get_mat()[i][j] = sRGB_TO_XYZ_MAT[i][j];
        }
    }

    return linear_colorspace_change<double>(input, trans);
}

Image<double> xyz_to_srgb(const Image<double> &input) {
    Matrix<double> trans(3, 3);
    for (int i = 0; i < trans.get_rows(); i++) {
        for (int j = 0; j < trans.get_cols(); j++) {
            trans.get_mat()[i][j] = XYZ_TO_sRGB_MAT[i][j];
        }
    }

    return linear_colorspace_change<double>(input, trans);
}

Image<uint8_t> adjust_brightness_rgb(const Image<uint8_t> &input, int bias) {
    uint8_t lut[256];
    create_lookup_table<uint8_t>(lut, [=] (int num) -> uint8_t  {
        return static_cast<uint8_t>(num + bias);
    });

    return point_op_lut<uint8_t>(input, lut);
}

Image<uint8_t> adjust_contrast_rgb(const Image<uint8_t> &input, float gain) {
    uint8_t lut[256];
    create_lookup_table<uint8_t>(lut, [=] (int num) -> uint8_t {
        return static_cast<uint8_t>(num * gain + 0.5);
    });

    return point_op_lut<uint8_t>(input, lut);
}

Image<uint8_t> adjust_brightness_xyz(const Image<uint8_t> &input, int bias) {
    double adj_bias = static_cast<double>(bias) / 255;

    Image<double> output = srgb_to_xyz(linearize_srgb(input));
    for (double *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *(i + 1) += adj_bias;
    }

    return unlinearize_srgb(xyz_to_srgb(output));
}

Image<uint8_t> adjust_contrast_xyz(const Image<uint8_t> &input, float gain) {
    Image<double> output = srgb_to_xyz(linearize_srgb(input));
    for (double *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *(i + 1) *= gain;
    }

    return unlinearize_srgb(xyz_to_srgb(output));
}

//Image<uint8_t> full_histogram_eq(const Image<uint8_t> &input) {
//
//}
//
//Image<uint8_t> partial_histogram_eq(const Image<uint8_t> &input) {
//
//}
