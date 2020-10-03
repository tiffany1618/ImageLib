#ifndef PHOTOEDITOR_CORE_H
#define PHOTOEDITOR_CORE_H

#include <string>

#include "Image.hpp"

// Colorspace changes
Image<uint8_t> to_grayscale(const Image<uint8_t> &input);
Image<double> linearize_srgb(const Image<uint8_t> &input);
Image<uint8_t> unlinearize_srgb(const Image<double> &input);
Image<double> srgb_to_xyz(const Image<double> &input);
Image<double> xyz_to_srgb(const Image<double> &input);
Image<double> xyz_to_lab(const Image<double> &input, const std::string &ref_white);
Image<double> lab_to_xyz(const Image<double> &input, const std::string &ref_white);
Image<double> rgb_to_hsv(const Image<uint8_t> &input);
Image<uint8_t> hsv_to_rgb(const Image<double> &input);

// Brightness/contrast correction
Image<uint8_t> adjust_brightness_rgb(const Image<uint8_t> &input, int bias);
Image<uint8_t> adjust_contrast_rgb(const Image<uint8_t> &input, float gain);
Image<uint8_t> adjust_brightness_xyz(const Image<uint8_t> &input, int bias);
Image<uint8_t> adjust_contrast_xyz(const Image<uint8_t> &input, float gain);
Image<uint8_t> histogram_equalization(const Image<uint8_t> &input, double alpha, const std::string &ref_white);
Image<uint8_t> adjust_saturation(const Image<uint8_t> &input, int val);

#endif //PHOTOEDITOR_CORE_H
