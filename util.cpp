//
// Created by tiffany on 9/22/20.
//

#include <iostream>

#include "util.h"

// TODO: What do to about values out of range?
void create_lookup_table(uint8_t *lut, func_lut f) {
    for (int i = 0; i < 255; i++) {
        lut[i] = f(i);
    }
}

Image point_op_lut(const Image &input, const uint8_t *lut) {
    Image output(input.get_width(), input.get_height(), input.get_channels());

    for (uint8_t *i = input.get_data(), *j = output.get_data(); i != input.get_data() + input.get_size();
        i += input.get_channels(), j += output.get_channels()) {
        *j = lut[*i];
        *(j + 1) = lut[*(i + 1)];
        *(j + 2) = lut[*(i + 2)];

        // Copy over transparency channel if it exists
        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
}

Image colorspace_change(const Image &input, const Matrix<double> &mat) {
    Image output(input.get_width(), input.get_height(), input.get_channels());
    Matrix<double> rgb(3, 1);
    Matrix<double> temp(3, 1);

    for (uint8_t *i = input.get_data(), *j = output.get_data(); i != input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        rgb.get_mat()[0][0] = static_cast<double>(*i);
        rgb.get_mat()[1][0] = static_cast<double>(*(i + 1));
        rgb.get_mat()[2][0] = static_cast<double>(*(i + 2));

        temp = mat * rgb;

        *j = static_cast<uint8_t>(temp.get_mat()[0][0] + 0.5);
        *(j + 1) = static_cast<uint8_t>(temp.get_mat()[1][0] + 0.5);
        *(j + 2) = static_cast<uint8_t>(temp.get_mat()[2][0] + 0.5);

        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
}
