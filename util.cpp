#include <cmath>

#include "util.h"

void lab_xyz_white_point_vals(std::string ref_white, double &x_n, double &y_n, double &z_n) {
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
