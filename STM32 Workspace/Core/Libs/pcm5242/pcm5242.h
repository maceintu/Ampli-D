#ifndef LIBS_PCM5242_PCM5242_H_
#define LIBS_PCM5242_PCM5242_H_

#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

// controle du volume
#define PCM5242_VOL_PAGE        0x00u   /* les registres de volume sont en page 0 */
#define PCM5242_REG_VOL_L    	0x3Du
#define PCM5242_REG_VOL_R    	0x3Eu
#define PCM5242_MIN_ATTENUATION 0x3Cu
#define PCM5242_MAX_ATTENUATION 0xFEu
#define PCM5242_VOL_MUTE 		0xFFu
#define PCM5242_BALANCE_RANGE 	80

/* Adresse 7 bits du PCM5242 sur la carte Ampli-D (ADR1 et ADR2 a la masse). */
#define PCM5242_ADDR            0x4Cu

/* Le HAL attend l'adresse deja decalee d'un bit vers la gauche. */
#define PCM5242_HAL_ADDR        ((uint16_t)(PCM5242_ADDR << 1))

/* Delai d'attente I2C, en ms. */
#define PCM5242_TIMEOUT         50u

/* 0x00 - Page select. Present a l'offset 0 de toutes les pages. */
#define PCM5242_PAGE_UNKNOWN    0xFFu
#define PCM5242_REG_PAGE        0x00u

HAL_StatusTypeDef pcm5242_readRegister(I2C_HandleTypeDef *hi2c, uint8_t page,
		uint8_t reg, uint8_t *value);

HAL_StatusTypeDef pcm5242_writeRegister(I2C_HandleTypeDef *hi2c, uint8_t page,
		uint8_t reg, uint8_t value);

HAL_StatusTypeDef set_audio_level_and_balance(I2C_HandleTypeDef *hi2c, uint8_t volume,
		uint8_t balance);

HAL_StatusTypeDef pcm5242_mute(I2C_HandleTypeDef *hi2c);

#endif /* LIBS_PCM5242_PCM5242_H_ */
