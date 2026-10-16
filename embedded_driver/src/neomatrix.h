#ifndef NEOMATRIX_H

#include <new>
#include <cstdint>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define NEOMATRIX_H

class NeoMatrix
{

    uint32_t pixels[13 * 13];
    /**
     * Pixel buffer for the NeoMatrix. Each element contains the color
     * data for pixel (row, col). Maximum of 169 elements.
     */

    uint8_t num_rows, num_cols;
    /**
     * Metadata for pixel buffer size. 
     */


    PIO pio;
    uint8_t sm;
    uint8_t offset;
    bool initialized;

public:

    /**
     * @brief Constructor. Initializes the *pixels* matrix.
     *
     * @param width Number of columns of the LED matrix.
     * @param height Number of rows of the LED matrix.
     *
     * @return None.
     */
    NeoMatrix(uint8_t width, uint8_t height);
    
    /**
     * @brief Initializes the LED matrix.
     *
     * @return true: Initialization successful.
     * @return false: Initialization failed.
     */
    bool init(void);

    /**
     * @brief Sets an LED to a color.
     *
     * @param row Row of the LED to be set.
     * @param col Column of the LED to be set.
     * @param color RGB values of the desired color.
     *
     * @return None.
     */
    void set_pixel(uint8_t row, uint8_t col, uint32_t color);

    /**
     * @brief Sets all elements of the pixel buffer to 0x00.
     *
     * @return None.
     */
    void clear_pixels(void);

    /**
     * @brief Write the pixel buffer to the NeoMatrix
     *
     * @return None.
     */
    void write(void);

};


#endif