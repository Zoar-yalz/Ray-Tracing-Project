#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void inline write_color(std::ostream& out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}
void inline write_color(char* data, int w, int h,int x,int y ,color pixel_color)
{
    const int comp = 3;
    data[w * comp * x + comp * y + 0] = static_cast<char>(255.999 * pixel_color.x());
    data[w * comp * x + comp * y + 1] = static_cast<char>(255.999 * pixel_color.y());
    data[w * comp * x + comp * y + 2] = static_cast<char>(255.999 * pixel_color.z());
}

#endif