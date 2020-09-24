//
// Created by tiffany on 9/22/20.
//

#ifndef PHOTOEDITOR_CORE_H
#define PHOTOEDITOR_CORE_H

#include "Image.h"

// Color changes
Image to_grayscale(const Image &input);
Image rgb_to_xyz(const Image &input);
Image xyz_to_rgb(const Image &input);

// Manual color correction
Image adjust_brightness_rgb(const Image &input, int bias);
Image adjust_contrast_rgb(const Image &input, float gain); // gain > 0
Image adjust_brightness_xyz(const Image &input, int bias);
Image adjust_contrast_xyz(const Image &input, float gain); // gain > 0

// Automatic color correction
Image full_histogram_eq(const Image &input);
Image partial_histogram_eq(const Image &input);

#endif //PHOTOEDITOR_CORE_H
