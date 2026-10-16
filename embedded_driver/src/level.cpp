
#include "lis3dh.h"
#include "neomatrix.h"

#include "pico/stdlib.h"

#include <cmath>
#include <cstdint>

namespace
{
    constexpr uint8_t MATRIX_WIDTH  = 8;
    constexpr uint8_t MATRIX_HEIGHT = 8;

    constexpr float LEVEL_TOLERANCE = 0.1f;

    constexpr uint32_t RED   = 0xFF0000;
    constexpr uint32_t GREEN = 0x00FF00;

    constexpr uint LED_PIN = 13;
    constexpr bool LED_ON  = true;
    constexpr uint32_t BLINK_INTERVAL_MS = 1000;


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
            // x axis tilt
            uint8_t col = (x > 0.0f) ? 7 : 0;
            neomatrix.set_pixel(3, col, RED);
        }
        else
        {
            // y axis tilt
            uint8_t row = (y > 0.0f) ? 0 : 7;
            neomatrix.set_pixel(row, 3, RED);
        }

        neomatrix.write();
    }
}

int main()
{
    stdio_init_all();
    sleep_ms(250);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    LIS3DH lis3dh;
    NeoMatrix neomatrix(MATRIX_WIDTH, MATRIX_HEIGHT);

    if (!lis3dh.init())
    {
        gpio_put(LED_PIN, LED_ON);
    }
    
    if (!neomatrix.init())
    {
        gpio_put(LED_PIN, LED_ON);
    }

    while (true)
    {
        lis3dh.update();

        display_level(neomatrix, lis3dh.get_x(), lis3dh.get_y());

        sleep_ms(50);
    }

    return 0;
}
