#ifndef PHOTOEDITOR_IMAGE_H
#define PHOTOEDITOR_IMAGE_H

#include <cstdint>
#include <cstddef>

template<typename T>
class Image {
public:
    explicit Image(const char *filename);
    Image(int width, int height, int channels);
    Image(const Image<T> &img);
    ~Image();
    Image<T>& operator=(const Image<T> &img);

    int get_height() const;
    int get_width() const;
    int get_channels() const;
    int get_size() const;
    T* get_data() const;

    // Write to file
    void write_png(const char *filename) const;
    void write_jpg(const char *filename, int quality) const;

private:
    int height;
    int width;
    int channels;
    int size;
    T *data;
};

#endif //PHOTOEDITOR_IMAGE_H
