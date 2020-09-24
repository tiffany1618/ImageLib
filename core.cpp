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
    Matrix<double> trans(3, 3);
    for (int i = 0; i < trans.get_rows(); i++) {
        for (int j = 0; j < trans.get_cols(); j++) {
            trans.get_mat()[i][j] = RGB_TO_XYZ_MAT[i][j];
        }
    }

    return colorspace_change(input, trans);
}

Image xyz_to_rgb(const Image &input) {
    Matrix<double> trans(3, 3);
    for (int i = 0; i < trans.get_rows(); i++) {
        for (int j = 0; j < trans.get_cols(); j++) {
            trans.get_mat()[i][j] = XYZ_TO_RGB_MAT[i][j];
        }
    }

    return colorspace_change(input, trans);
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

Image adjust_brightness_xyz(const Image &input, int bias) {
    uint8_t lut[256];
    create_lookup_table(lut, [=] (int num) -> uint8_t {
       return static_cast<uint8_t>(num + bias);
    });

    Image output = rgb_to_xyz(input);
    for (uint8_t *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *(i + 1) = lut[*(i + 1)];
    }

    return xyz_to_rgb(output);
}

Image adjust_contrast_xyz(const Image &input, float gain) {
    uint8_t lut[256];
    create_lookup_table(lut, [=] (int num) -> uint8_t {
        return static_cast<uint8_t>(num * gain);
    });

    Image output = rgb_to_xyz(input);
    for (uint8_t *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *(i + 1) = lut[*(i + 1)];
    }

    return xyz_to_rgb(output);
}
