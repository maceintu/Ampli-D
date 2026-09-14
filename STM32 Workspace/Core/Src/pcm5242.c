#include "pcm5242.h"

HAL_StatusTypeDef pcm5242_read_reg(I2C_HandleTypeDef *hi2c,
                                   uint8_t reg,
                                   uint8_t *value)
{
    if ((hi2c == NULL) || (value == NULL)) {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Read(hi2c, PCM5242_HAL_ADDR,
                            reg, I2C_MEMADD_SIZE_8BIT,
                            value, 1u, PCM5242_TIMEOUT);
}

HAL_StatusTypeDef pcm5242_write_reg(I2C_HandleTypeDef *hi2c,
                                    uint8_t reg,
                                    uint8_t value)
{
    if (hi2c == NULL) {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Write(hi2c, PCM5242_HAL_ADDR,
                             reg, I2C_MEMADD_SIZE_8BIT,
                             &value, 1u, PCM5242_TIMEOUT);
}
