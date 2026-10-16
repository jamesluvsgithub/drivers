#include "neomatrix.h"




namespace 
{
    constexpr uint WS2812_PIN = 16;
    constexpr uint WS2812_POWER_PIN = 10;
    constexpr float WS2812_FREQ = 800000.0f;
    constexpr bool RGBW_OFF = false;
}



NeoMatrix::NeoMatrix(uint8_t width, uint8_t height)
{
    num_rows = height;
    num_cols = width;
    pio = pio0;
    sm = 0;
    offset = 0;
    initialized = false;
}


bool NeoMatrix::init(void)
{
    gpio_init(WS2812_POWER_PIN);
    gpio_set_dir(WS2812_POWER_PIN, GPIO_OUT);
    gpio_put(WS2812_POWER_PIN, 1);

    // Load the WS2812 program into PIO instruction memory.
    if (!pio_can_add_program(pio, &ws2812_program)) 
    {
        gpio_put(WS2812_POWER_PIN, 0);
        return false;
    }

    offset = pio_add_program(pio, &ws2812_program);

    // Claim an available PIO state machine.
    int claimed_sm = pio_claim_unused_sm(pio, false);

    if (claimed_sm < 0) 
    {
        pio_remove_program(pio, &ws2812_program, offset);
        gpio_put(WS2812_POWER_PIN, 0);
        return false;
    }

    sm = static_cast<uint>(claimed_sm);

    // false = RGB (24 bits), not RGBW (32 bits).
    ws2812_program_init(pio, sm, offset, WS2812_PIN, WS2812_FREQ, RGBW_OFF);

    initialized = true;
    clear_pixels();
    write();

    return true;
}


void NeoMatrix::set_pixel(uint8_t row, uint8_t col, uint32_t color)
{
    if (row >= num_rows || col >= num_cols) 
    {
        return;
    }

    uint16_t address = row * num_cols + col;
    pixels[address] = color & 0x00FFFFFFu;
}


void NeoMatrix::clear_pixels(void)
{
    for (uint16_t i = 0; i < num_rows * num_cols; ++i) 
    {
        pixels[i] = 0x0000;
    }
}


void NeoMatrix::write(void)
{
    if (!initialized) 
    {
        return;
    }

    for (uint16_t i = 0; i < num_rows * num_cols; ++i) 
    {
        uint32_t color = pixels[i];

        // Input color format: 0xRRGGBB.
        uint32_t r = (color >> 16) & 0xFF;
        uint32_t g = (color >> 8)  & 0xFF;
        uint32_t b = color & 0xFF;

        // WS2812 expects GRB ordering.
        uint32_t grb = (g << 16) | (r << 8) | b;

        // The PIO program shifts out the 24-bit pixel value.
        pio_sm_put_blocking(pio, sm, grb << 8);
    }

    sleep_us(80);
}