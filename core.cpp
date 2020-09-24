//
// Created by tiffany on 9/22/20.
//

#include <iostream>

#include "core.h"
#include "util.h"
#include "Matrix.h"

Image to_grayscale(const Image &input) {
    int channels;
    if (input.get_channels() == 4) {
        channels = 2;
    } else {
        channels = 1;
    }

    Image output(input.get_width(), input.get_height(), channels);

    for (uint8_t *i = input.get_data(), *j = output.get_data(); i != input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        *j = (*i + (*i + 1) + *(i + 2)) / 3.0;

        if (input.get_channels() == 4) {
            *(j + 1) = *(i + 3);
        }
    }

    return output;
}

Image rgb_to_xyz(const Image &input) {
    Image output(input.get_width(), input.get_height(), input.get_channels());
    Matrix<double> trans(3, 3);
    Matrix<double> rgb(3, 1);
    Matrix<double> temp(3, 1);

    // Populate transformation matrix
    for (int i = 0; i < trans.get_rows(); i++) {
        for (int j = 0; j < trans.get_cols(); j++) {
            trans.get_mat()[i][j] = RGB_TO_XYZ_MAT[i][j];
        }
    }
    trans = trans * RGB_TO_XYZ_SCAL;

    for (uint8_t *i = input.get_data(), *j = output.get_data(); i != input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        rgb.get_mat()[0][0] = *i;
        rgb.get_mat()[0][1] = *(i + 1);
        rgb.get_mat()[0][2] = *(i + 2);

        temp = trans * rgb;

        *j = static_cast<uint8_t>(temp.get_mat()[0][0]);
        *(j + 1) = static_cast<uint8_t>(temp.get_mat()[0][1]);
        *(j + 2) = static_cast<uint8_t>(temp.get_mat()[0][2]);

        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
}

Image adjust_brightness_rgb(const Image &input, int bias) {
    uint8_t lut[256];
    create_lookup_table(lut, [=] (int num) -> uint8_t  {
        return static_cast<uint8_t>(num + bias);
    });

    return point_op_lut(input, lut);
}

Image adjust_contrast_rgb(const Image &input, float gain) {
    uint8_t lut[256];
    create_lookup_table(lut, [=] (int num) -> uint8_t {
        return static_cast<uint8_t>(num * gain);
    });

    return point_op_lut(input, lut);
}
