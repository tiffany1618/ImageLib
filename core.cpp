#include <iostream>
#include <cmath>
#include <map>

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

        if (num < 0) num = 0;
        if (num > UINT8_MAX) num = UINT8_MAX;

        return static_cast<uint8_t>(round(num));
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

Image<double> xyz_to_lab(const Image<double> &input, const std::string &ref_white) {
    double x_n, y_n, z_n;
    generate_xyz_tristimulus_vals(ref_white, x_n, y_n, z_n);

    Image<double> output(input.get_width(), input.get_height(), input.get_channels());
    double x, y, z;
    for (double *i = input.get_data(), *j = output.get_data(); i < input.get_data() + input.get_size();
        i += input.get_channels(), j += output.get_channels()) {
        x = xyz_to_lab_func((*i) * 100 / x_n);
        y = xyz_to_lab_func((*(i + 1)) * 100 / y_n);
        z = xyz_to_lab_func((*(i + 2)) * 100 / z_n);

        *j = 116.0 * y - 16;
        *(j + 1) = 500.0 * (x - y);
        *(j + 2) = 200.0 * (y - z);

        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
}

Image<double> lab_to_xyz(const Image<double> &input, const std::string &ref_white) {
    double x_n, y_n, z_n;
    generate_xyz_tristimulus_vals(ref_white, x_n, y_n, z_n);

    Image<double> output(input.get_width(), input.get_height(), input.get_channels());
    double n;
    for (double *i = input.get_data(), *j = output.get_data(); i < input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        n = ((*i) + 16) / 116.0;

        *j = x_n * lab_to_xyz_func(n + (*(i + 1)) / 500.0) / 100;
        *(j + 1) = y_n * lab_to_xyz_func(n) / 100;
        *(j + 2) = z_n * lab_to_xyz_func(n - (*(i + 2)) / 200.0) / 100;

        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
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
        return static_cast<uint8_t>(round(num * gain));
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

Image<uint8_t> full_histogram_eq(const Image<uint8_t> &input, const std::string &ref_white) {
    Image<double> output = xyz_to_lab(srgb_to_xyz(linearize_srgb(input)), ref_white);
    std::map<double, double> percentiles;
    generate_histogram_percentiles(output, percentiles);

    for (double *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *i = percentiles[*i] * 100;
    }

    return unlinearize_srgb(xyz_to_srgb(lab_to_xyz(output, ref_white)));
}

// alpha = 0 corresponds to no equalization
// alpha = 1 corresponds to full equalization
Image<uint8_t> partial_histogram_eq(const Image<uint8_t> &input, double alpha, const std::string &ref_white) {
    Image<double> output = xyz_to_lab(srgb_to_xyz(linearize_srgb(input)), ref_white);
    std::map<double, double> percentiles;
    generate_histogram_percentiles(output, percentiles);

    for (double *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *i = (alpha * percentiles[*i] * 100) + ((1.0 - alpha) * (*i));
    }

    return unlinearize_srgb(xyz_to_srgb(lab_to_xyz(output, ref_white)));
}
