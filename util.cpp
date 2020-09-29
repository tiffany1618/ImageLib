#include <cmath>

#include "util.h"

void generate_xyz_tristimulus_vals(const std::string &ref_white, double &x_n, double &y_n, double &z_n) {
    if (ref_white == "D50" || ref_white == "d50") {
        x_n = 96.4212;
        y_n = 100;
        z_n = 82.5188;
    } else if (ref_white == "D65" || ref_white == "d65") {
        x_n = 95.0489;
        y_n = 100;
        z_n = 103.8840;
    } else {
        std::cout << "Invalid reference white point" << std::endl;
    }
}

double xyz_to_lab_func(double num) {
    double d = 6.0 / 29;

    if (num > pow(d, 3)) {
        return pow(num, 1.0 / 3);
    } else {
        return (num / (3 * d * d)) + (4.0 / 29);
    }
}

double lab_to_xyz_func(double num) {
    double d = 6.0 / 29;

    if (num > d) {
        return pow(num, 3);
    } else {
        return (3 * d * d) * (num - 4.0 / 29);
    }
}

void generate_histogram_percentiles(const Image<double> &input, std::map<double, double> &percentiles) {
    std::map<double, int> histogram;

    for (double *i = input.get_data(); i < input.get_data() + input.get_size(); i += input.get_channels()) {
        if (histogram.find(*i) == histogram.end()) {
            histogram[*i] = 1;
        } else {
            histogram[*i]++;
        }
    }

    int sum = 0;
    int num_pixels = input.get_width() * input.get_height();
    for (auto i = histogram.begin(); i != histogram.end(); i++) {
        sum += i->second;
        percentiles[i->first] = static_cast<double>(sum) / num_pixels;
    }
}
