
#include "lis3dh.h"
#include "neomatrix.h"

#include "pico/stdlib.h"

#include <cmath>
#include <cstdint>

namespace
{
    constexpr uint8_t MATRIX_WIDTH = 8;
    constexpr uint8_t MATRIX_HEIGHT = 8;

    constexpr float LEVEL_TOLERANCE = 0.1f;

    constexpr uint32_t RED   = 0xFF0000;
    constexpr uint32_t GREEN = 0x00FF00;


    void display_level(NeoMatrix& neomatrix, float x, float y)
    {
        neomatrix.clear_pixels();

        if (std::fabs(x) <= LEVEL_TOLERANCE &&
            std::fabs(y) <= LEVEL_TOLERANCE)
        {
            neomatrix.set_pixel(3, 3, GREEN);
            neomatrix.set_pixel(3, 4, GREEN);
            neomatrix.set_pixel(4, 3, GREEN);
            neomatrix.set_pixel(4, 4, GREEN);
        }
        else if (std::fabs(x) >= std::fabs(y))
        {
            // X-axis tilt: one red LED on the left or right.
            uint8_t col = (x > 0.0f) ? 7 : 0;
            neomatrix.set_pixel(3, col, RED);
        }
        else
        {
            // Y-axis tilt: one red LED at the top or bottom.
            uint8_t row = (y > 0.0f) ? 0 : 7;
            neomatrix.set_pixel(row, 3, RED);
        }

        neomatrix.write();
    }
}

int main()
{
    stdio_init_all();

    LIS3DH lis3dh;
    NeoMatrix neomatrix(MATRIX_WIDTH, MATRIX_HEIGHT);

    if (!lis3dh.init())
    {
        while (true)
            tight_loop_contents();
    }

    if (!neomatrix.init())
    {
        while (true)
            tight_loop_contents();
    }

    while (true)
    {
        lis3dh.update();

        display_level(neomatrix, lis3dh.get_x(), lis3dh.get_y());

        sleep_ms(50);
    }

    return 0;
}
