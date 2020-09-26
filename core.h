#ifndef PHOTOEDITOR_CORE_H
#define PHOTOEDITOR_CORE_H

#include "Image.h"

// Colorspace changes
Image to_grayscale(const Image &input);
Image linearize_srgb(const Image &input);
Image unlinearize_srgb(const Image &input);
Image srgb_to_xyz(const Image &input);
Image xyz_to_srgb(const Image &input);
Image xyz_to_lab(const Image &input);
Image lab_to_xyz(const Image &input);

// Manual color correction
Image adjust_brightness_rgb(const Image &input, int bias);
Image adjust_contrast_rgb(const Image &input, float gain); // gain > 0
Image adjust_brightness_xyz(const Image &input, int bias);
Image adjust_contrast_xyz(const Image &input, float gain); // gain > 0

// Automatic color correction
Image full_histogram_eq(const Image &input);
Image partial_histogram_eq(const Image &input);

#endif //PHOTOEDITOR_CORE_H
