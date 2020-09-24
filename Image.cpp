#include <iostream>

#include "Image.h"
#include "util.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

Image::Image(const char *filename) {
    uint8_t *temp = stbi_load(filename, &width, &height, &channels, 0);
    size = width * height * channels;
    this->data = new uint8_t[size];

    for (int i = 0; i < size; i++) {
        data[i] = temp[i];
    }
}

Image::Image(int width, int height, int channels)
    : width(width), height(height), channels(channels), size(width * height * channels) {
    this->data = new uint8_t[size];
}

Image::Image(const Image &img)
    : width(img.width), height(img.height), channels(img.channels), size(width * height * channels) {
    this->data = new uint8_t[this->size];

    for (int i = 0; i < this->size; i++) {
        this->data[i] = img.data[i];
    }
}

Image::~Image() {
    delete[] data;
}

Image& Image::operator=(const Image &img) {
    if (this != &img) {
        this->width = img.width;
        this->height = img.height;
        this->channels = img.channels;
        this->size = img.size;

        for (int i = 0; i < this->size; i++) {
            this->data[i] = img.data[i];
        }
    }

    return *this;
}

int Image::get_height() const {
    return height;
}

int Image::get_width() const {
    return width;
}

int Image::get_channels() const {
    return channels;
}

int Image::get_size() const {
    return size;
}

uint8_t* Image::get_data() const {
    return data;
}

void Image::write_png(const char *filename) const {
    stbi_write_png(filename, width, height, channels, data, width * channels);
}

void Image::write_jpg(const char *filename, int quality) const {
    stbi_write_jpg(filename, width, height, channels, data, quality);
}

