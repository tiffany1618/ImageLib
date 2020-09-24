#include <iostream>

#include "Image.h"
#include "core.h"

using std::cout;
using std::cin;
using std::endl;

int main() {
    Image img("../images/malibu.jpg");

//    Image test1 = to_grayscale(img);
//    test1.write_jpg("../images/malibu_gray.jpg", 100);
//    Image test2 = adjust_brightness_rgb(img, 16);
//    test2.write_jpg("../images/malibu_b16", 100);
//    Image test3 = adjust_contrast_rgb(img, 1.1);
//    test3.write_jpg("../images/malibu_c11", 100);

    Image test4 = rgb_to_xyz(img);
    test4.write_jpg("../images/malibu_xyz.jpg", 100);

    return 0;
}
