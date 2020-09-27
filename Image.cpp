#include <iostream>

#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

template<typename T>
Image<T>::Image(const char *filename) {
    uint8_t *temp = stbi_load(filename, &width, &height, &channels, 0);
    size = width * height * channels;
    this->data = new T[size];

    for (int i = 0; i < size; i++) {
        data[i] = static_cast<T>(temp[i]);
    }
}

template<typename T>
Image<T>::Image(int width, int height, int channels)
    : width(width), height(height), channels(channels), size(width * height * channels) {
    this->data = new T[size];
}

template<typename T>
Image<T>::Image(const Image<T> &img)
    : width(img.width), height(img.height), channels(img.channels), size(width * height * channels) {
    this->data = new T[this->size];

    for (int i = 0; i < this->size; i++) {
        this->data[i] = img.data[i];
    }
}

template<typename T>
Image<T>::~Image() {
    delete[] data;
}

template<typename T>
Image<T>& Image<T>::operator=(const Image<T> &img) {
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

template<typename T>
int Image<T>::get_height() const {
    return height;
}

template<typename T>
int Image<T>::get_width() const {
    return width;
}

template<typename T>
int Image<T>::get_channels() const {
    return channels;
}

template<typename T>
int Image<T>::get_size() const {
    return size;
}

template<typename T>
T* Image<T>::get_data() const {
    return data;
}

template<typename T>
void Image<T>::write_png(const char *filename) const {
    if (std::is_same<T, uint8_t>::value) {
        stbi_write_png(filename, width, height, channels, data, width * channels);
    } else {

    }
}

template<typename T>
void Image<T>::write_jpg(const char *filename, int quality) const {
    stbi_write_jpg(filename, width, height, channels, data, quality);
}

template class Image<uint8_t>;
template class Image<double>;

