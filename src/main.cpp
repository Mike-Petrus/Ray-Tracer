#include "color.h"
#include "vec3.h"

#include <iostream>

int main()
{
    // Image
    int image_width = 256;
    int image_height = 256;

    // Render

    std::cout << "P3\n" << image_width <<  ' ' << image_height << "\n255\n";

    for (int i = 0; i < image_height; i++)
    {
        std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;

        for (int j = 0; j < image_width; j++)
        {
            auto pixel_color = color(double(j)/(image_width - 1), double(i)/(image_height - 1), 0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.       \n";
    return 0;
}