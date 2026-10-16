#include "lis3dh.h" 


namespace
{
    constexpr uint8_t SDA_PIN = 2;
    constexpr uint8_t SCL_PIN = 3; 
}



LIS3DH::LIS3DH(void)
{
    x = y = z = 0.0;
}

    
bool LIS3DH::init(void)
{
    /*
    Initializing I2C1
    */
    i2c_init(i2c1, 400 * 1000);  // 400 kHz

    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // Optional internal pull-ups.
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    


    /*
    Initializing the LIS3DH.
    */

    // Check WHO_AM_I register
    if (read_reg(WHO_AM_I_ADDR) != 0x33)
    {
        return false;
    }

    // Normal mode, 1.344kHz data rate, enable X, Y, and Z axes.
    set_reg(CTRL_REG_1_ADDR, 0x97);

    // Block data on.
    set_reg(CTRL_REG_4_ADDR, 0x80);

    return true;
}


void LIS3DH::set_reg(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(i2c1, LIS3DH_ADDR, buf, 2, false);
}


uint8_t LIS3DH::read_reg(uint8_t reg)
{   
    uint8_t val;
    i2c_write_blocking(i2c1, LIS3DH_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c1, LIS3DH_ADDR, &val, 1, false);
    
    return val;
}


void LIS3DH::update(void)
{
    float sensitivity = 0.004f;
    float scaling = 64 / sensitivity;
    float* axes[3] = { &x, &y, &z };
    uint8_t OUTPUT_REG_L = OUT_X_L_ADDR;
    uint8_t OUTPUT_REG_H = OUT_X_H_ADDR;

    uint8_t lsb;
    uint8_t msb;
    uint16_t raw_acceleration;

    for (uint8_t i = 0; i < 3; ++i)
    {
        lsb = read_reg(OUTPUT_REG_L + 2*i);
        msb = read_reg(OUTPUT_REG_H + 2*i);
        raw_acceleration = (msb << 8) | lsb;

        *axes[i] = (float) ((int16_t) raw_acceleration) / scaling;
    }
}

float LIS3DH::get_x()
{
    return x;
}

float LIS3DH::get_y()
{
    return y;
}

float LIS3DH::get_z()
{
    return z;
}