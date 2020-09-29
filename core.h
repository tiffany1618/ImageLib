#ifndef PHOTOEDITOR_CORE_H
#define PHOTOEDITOR_CORE_H

#include <string>

#include "Image.h"

// Colorspace changes
Image<uint8_t>to_grayscale(const Image<uint8_t>&input);
Image<double> linearize_srgb(const Image<uint8_t>&input);
Image<uint8_t>unlinearize_srgb(const Image<double> &input);
Image<double> srgb_to_xyz(const Image<double> &input);
Image<double> xyz_to_srgb(const Image<double> &input);
Image<double> xyz_to_lab(const Image<double> &input, std::string ref_white);
Image<double> lab_to_xyz(const Image<double> &input, std::string ref_white);

// Manual color correction
Image<uint8_t>adjust_brightness_rgb(const Image<uint8_t>&input, int bias);
Image<uint8_t>adjust_contrast_rgb(const Image<uint8_t>&input, float gain); // gain > 0
Image<uint8_t>adjust_brightness_xyz(const Image<uint8_t>&input, int bias);
Image<uint8_t>adjust_contrast_xyz(const Image<uint8_t>&input, float gain); // gain > 0

// Automatic color correction
Image<uint8_t>full_histogram_eq(const Image<uint8_t>&input);
Image<uint8_t>partial_histogram_eq(const Image<uint8_t>&input);

#endif //PHOTOEDITOR_CORE_H
