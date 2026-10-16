#ifndef LIS3DH_H
#define LIS3DH_H

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"


class LIS3DH
{
public:

    /**
     * Relevant LIS3DH addresses.
     */
    static constexpr int LIS3DH_ADDR = 0x18;
    static constexpr uint8_t WHO_AM_I_ADDR = 0x0F;
    static constexpr uint8_t CTRL_REG_1_ADDR = 0x20;
    static constexpr uint8_t CTRL_REG_4_ADDR = 0x23;

    /**
     * LIS3DH output register addresses.
     */
    static constexpr uint8_t OUT_X_L_ADDR = 0x28;
    static constexpr uint8_t OUT_X_H_ADDR = 0x29;
    static constexpr uint8_t OUT_Y_L_ADDR = 0x2A;
    static constexpr uint8_t OUT_Y_H_ADDR = 0x2B;
    static constexpr uint8_t OUT_Z_L_ADDR = 0x2C;
    static constexpr uint8_t OUT_Z_H_ADDR = 0x2D;

private:

    float x, y, z;
    /**
     * Acceleration values in the x, y, and z planes respectively.
     */

public:

    /**
     * @brief Constructor.
     *
     * @return None.
     */
    LIS3DH(void);

    /**
     * @brief Initializes the accelerometer.
     *
     * @return true: Initialization successful.
     * @return false: Initialization failed.
     */
    bool init(void);

    /**
     * @brief Sets a register on the LIS3DH to a specified value.
     *
     * @param reg Address of the register to be written to.
     * @param val The value that will be written.
     *
     * @return None.
     */
    void set_reg(uint8_t reg, uint8_t val);

    /**
     * @brief Reads and returns a register's value on the LIS3DH.
     *
     * @param reg Address of the register to be read from.
     *
     * @return The byte at address *reg*.
     */
    uint8_t read_reg(uint8_t reg);

    /**
     * @brief Updates the class members x, y, and z with the current acceleration values.
     *
     * @return None.
     */
    void update(void);



    /**
     * @brief Gets the value x.
     *
     * @return x
     */
    float get_x();

    /**
     * @brief Gets the value y.
     *
     * @return y
     */
    float get_y();

    /**
     * @brief Gets the value z.
     *
     * @return z
     */
    float get_z();
};



#endif