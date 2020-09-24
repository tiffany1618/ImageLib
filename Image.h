//
// Created by tiffany on 9/22/20.
//

#ifndef PHOTOEDITOR_IMAGE_H
#define PHOTOEDITOR_IMAGE_H

#include <cstdint>
#include <cstddef>

#include "Matrix.h"

class Image {
public:
    Image(const char *filename);
    Image(int width, int height, int channels);
    Image(const Image &img);
    ~Image();
    Image& operator=(const Image &img);

    int get_height() const;
    int get_width() const;
    int get_channels() const;
    int get_size() const;
    uint8_t* get_data() const;

    // Write to file
    void write_png(const char *filename) const;
    void write_jpg(const char *filename, int quality) const;

private:
    int height;
    int width;
    int channels;
    int size;
    uint8_t *data;
};

#endif //PHOTOEDITOR_IMAGE_H
