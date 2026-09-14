#ifndef PCM5242_H
#define PCM5242_H

#include "stm32g0xx_hal.h"
#include <stdint.h>

// Adresse 7 bits du PCM5242 sur la carte Ampli-D (ADR1 et ADR2 a la masse).
#define PCM5242_ADDR        0x4Cu

// Le HAL attend l'adresse deja decalee d'un bit vers la gauche.
#define PCM5242_HAL_ADDR    ((uint16_t)(PCM5242_ADDR << 1))

//  Delai d'attente I2C, en ms.
#define PCM5242_TIMEOUT     50u

//  Registre de selection de page, present a l'offset 0 de toutes les pages.
#define PCM5242_REG_PAGE    0x00u

/**
 * @brief Lit un registre du PCM5242.
 * @param hi2c  bus I2C initialise par CubeMX, p.ex. &hi2c1
 * @param reg   numero de registre dans la page courante
 * @param value recoit la valeur lue
 * @return HAL_OK, ou HAL_ERROR / HAL_BUSY / HAL_TIMEOUT
 */
HAL_StatusTypeDef pcm5242_read_reg(I2C_HandleTypeDef *hi2c,
                                   uint8_t reg,
                                   uint8_t *value);

/**
 * @brief Ecrit un registre du PCM5242.
 * @param hi2c  bus I2C initialise par CubeMX, p.ex. &hi2c1
 * @param reg   numero de registre dans la page courante
 * @param value valeur a ecrire
 * @return HAL_OK, ou HAL_ERROR / HAL_BUSY / HAL_TIMEOUT
 */
HAL_StatusTypeDef pcm5242_write_reg(I2C_HandleTypeDef *hi2c,
                                    uint8_t reg,
                                    uint8_t value);

#endif /* PCM5242_H */
