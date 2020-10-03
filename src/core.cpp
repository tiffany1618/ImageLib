#include <iostream>
#include <cmath>
#include <map>
#include <algorithm>

#include "../include/core.hpp"
#include "../include/util.hpp"
#include "../include/Matrix.hpp"

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

// Input: sRGB range [0, 255]
// Output: sRGB range [0, 1] linearized
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

// Input: sRGB range [0, 1] linearized
// Output: sRGB range [0, 255]
Image<uint8_t> unlinearize_srgb(const Image<double> &input) {
    return point_op<double, uint8_t>(input, [] (double num) -> uint8_t {
        if (num <= 0.0031308) {
            num *= 3294.6;
        } else {
            num = 269.025 * pow(num, 1.0 /  2.4) - 14.025;
        }

        return cast_uint8_t(num);
    });
}

// Input: sRGB range [0, 1] linearized
// Output: CIEXYZ range [0, 1]
Image<double> srgb_to_xyz(const Image<double> &input) {
    Matrix<double> trans(3, 3);
    for (int i = 0; i < trans.get_rows(); i++) {
        for (int j = 0; j < trans.get_cols(); j++) {
            trans.get_mat()[i][j] = sRGB_TO_XYZ_MAT[i][j];
        }
    }

    return linear_colorspace_change<double>(input, trans);
}


// Input: CIEXYZ range [0, 1]
// Output: sRGB range [0, 1] linearized
Image<double> xyz_to_srgb(const Image<double> &input) {
    Matrix<double> trans(3, 3);
    for (int i = 0; i < trans.get_rows(); i++) {
        for (int j = 0; j < trans.get_cols(); j++) {
            trans.get_mat()[i][j] = XYZ_TO_sRGB_MAT[i][j];
        }
    }

    return linear_colorspace_change<double>(input, trans);
}

// Input: CIEXYZ range [0, 1]
// Output: CIELAB with L* range [0, 100]
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

// Input: CIELAB with L* range [0, 100]
// Output: CIEXYZ range [0, 1]
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

// Input: RGB range [0, 255]
// Output: HSV range [0, 1]
Image<double> rgb_to_hsv(const Image<uint8_t> &input) {
    Image<double> output(input.get_width(), input.get_height(), input.get_channels());

    double r, g, b, max, min, range;
    uint8_t *i;
    double *j;
    for (i = input.get_data(), j = output.get_data(); i < input.get_data() + input.get_size();
        i += input.get_channels(), j += output.get_channels()) {
        r = *i / 255.0;
        g = *(i + 1) / 255.0;
        b = *(i + 2) / 255.0;

        max = std::max({r, g, b});
        min = std::min({r, g, b});
        range = max - min;

        // Set value
        *(j + 2) = max;

        // Set saturation
        if (max == 0) {
            *(j + 1) = 0;
        } else {
            *(j + 1) = range / max;
        }

        // Set hue
        if (range == 0) {
            *j = 0;
        } else if (max == r) {
            *j = (g - b) / range;
        } else if (max == g) {
            *j = (b - r) / range + 2;
        } else {
            *j = (r - g) / range + 4;
        }

        *j /= 6.0;
        if (*j < 0) {
            (*j)++;
        } else if (*j > 1) {
            (*j)--;
        }

        std::cout << "rgb: " << r << ", " << b << ", " << g << std::endl;
        std::cout << "hsv: " << *j << ", " << *(j+1) << ", " << *(j+2) << std::endl;

        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
}

// Input: HSV range [0, 1]
// Ouput: RGB range [0, 255]
Image<uint8_t> hsv_to_rgb(const Image<double> &input) {
    Image<uint8_t> output(input.get_width(), input.get_height(), input.get_channels());

    double h, s, v, f;
    uint8_t p, q, t;
    double *i;
    uint8_t *j;
    for (i = input.get_data(), j = output.get_data(); i < input.get_data() + input.get_size();
         i += input.get_channels(), j += output.get_channels()) {
        h = (*i) * 6.0;
        s = *(i + 1);
        v = *(i + 2);

        if (s == 0) {
            v = cast_uint8_t(v * 255);
            *j = v;
            *(j + 1) = v;
            *(j + 2) = v;
        }

        f = h - floor(h);
        p = cast_uint8_t(v * (1 - s) * 255);
        q = cast_uint8_t(v * (1 - s * f) * 255);
        t = cast_uint8_t(v * (1 - s * (1 - f)) * 255);
        v = cast_uint8_t(v * 255);

        switch (static_cast<int>(h)) {
            case 0:
                *j = v;
                *(j + 1) = t;
                *(j + 2) = p;
                break;
            case 1:
                *j = q;
                *(j + 1) = v;
                *(j + 2) = p;
                break;
            case 2:
                *j = p;
                *(j + 1) = v;
                *(j + 2) = t;
                break;
            case 3:
                *j = p;
                *(j + 1) = q;
                *(j + 2) = v;
                break;
            case 4:
                *j = t;
                *(j + 1) = p;
                *(j + 2) = v;
                break;
            default:
                *j = v;
                *(j + 1) = p;
                *(j + 2) = q;
                break;
        }

        if (input.get_channels() == 4) {
            *(j + 3) = *(i + 3);
        }
    }

    return output;
}

// Adjusts brightness by adding the bias to each RGB channel
Image<uint8_t> adjust_brightness_rgb(const Image<uint8_t> &input, int bias) {
    uint8_t lut[256];
    create_lookup_table<uint8_t>(lut, [=] (int num) -> uint8_t  {
        return cast_uint8_t(num + bias);
    });

    return point_op_lut<uint8_t>(input, lut);
}

// Adjusts contrast by multiplying each RGB channel by the gain
// gain > 0
Image<uint8_t> adjust_contrast_rgb(const Image<uint8_t> &input, float gain) {
    uint8_t lut[256];
    create_lookup_table<uint8_t>(lut, [=] (int num) -> uint8_t {
        return cast_uint8_t(num * gain);
    });

    return point_op_lut<uint8_t>(input, lut);
}

// Adjusts brightness by adding the bias to the luminance value (Y)
Image<uint8_t> adjust_brightness_xyz(const Image<uint8_t> &input, int bias) {
    double adj_bias = static_cast<double>(bias) / 255;

    Image<double> output = srgb_to_xyz(linearize_srgb(input));
    for (double *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *(i + 1) += adj_bias;
    }

    return unlinearize_srgb(xyz_to_srgb(output));
}

// Adjusts contrast by multiplying the luminance value (Y) by the gain
// gain > 0
Image<uint8_t> adjust_contrast_xyz(const Image<uint8_t> &input, float gain) {
    Image<double> output = srgb_to_xyz(linearize_srgb(input));
    for (double *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *(i + 1) *= gain;
    }

    return unlinearize_srgb(xyz_to_srgb(output));
}

// alpha = 0 corresponds to no equalization
// alpha = 1 corresponds to full equalization
Image<uint8_t> histogram_equalization(const Image<uint8_t> &input, double alpha, const std::string &ref_white) {
    Image<double> output = xyz_to_lab(srgb_to_xyz(linearize_srgb(input)), ref_white);
    std::map<double, double> percentiles;
    generate_histogram_percentiles(output, percentiles);

    for (double *i = output.get_data(); i < output.get_data() + output.get_size(); i += output.get_channels()) {
        *i = (alpha * percentiles[*i] * 100) + ((1.0 - alpha) * (*i));
    }

    return unlinearize_srgb(xyz_to_srgb(lab_to_xyz(output, ref_white)));
}
