//
// Created by tiffany on 9/22/20.
//

#include "util.h"

// TODO: Oh this actually doesn't work
Image point_op(const Image &input, func_point_op f) {
    Image output(input.get_width(), input.get_height(), input.get_channels());

    for (int i = 0, j = 0; i != sizeof(input); i += input.get_channels(), j += output.get_channels()) {
        output.get_data()[j] = f(input.get_data()[i], input.get_data()[i+1], input.get_data()[i+2]);

        // Copy over transparency channel if it exists
        if (input.get_channels() == 4) {
            output.get_data()[j+3] = input.get_data()[j+3];
        }
    }

    return output;
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

// TODO: What do to about values out of range?
void create_lookup_table(uint8_t *lut, func_lut f) {
    for (int i = 0; i < 255; i++) {
        lut[i] = f(i);
    }
}