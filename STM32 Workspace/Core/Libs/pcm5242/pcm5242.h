#ifndef LIBS_PCM5242_PCM5242_H_
#define LIBS_PCM5242_PCM5242_H_

#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include "pcm5242/biquad.h"

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

#define DSP_FILTER_NUMBER 			3
#define PCM5242_BIQUAD_SCALE       8388608.0  // 2^23
#define PCM5242_BIQUAD_HALF_SCALE  4194304.0  // 2^22

typedef struct {
	uint8_t low;
	uint8_t medium;
	uint8_t high;
} Gains;

typedef struct {
	Gains gains;
	Shelf low_filter;
	Peaking medium_filter;
	Shelf high_filter;
} DSP_Manager;

typedef struct {
    int32_t n0;
    int32_t n1;
    int32_t n2;
    int32_t d1;
    int32_t d2;
} Custom_Coeffs;

HAL_StatusTypeDef pcm5242_read_register(I2C_HandleTypeDef *hi2c, uint8_t page, uint8_t reg, uint8_t *value);
HAL_StatusTypeDef pcm5242_write_register(I2C_HandleTypeDef *hi2c, uint8_t page, uint8_t reg, uint8_t value);

HAL_StatusTypeDef set_audio_level_and_balance(I2C_HandleTypeDef *hi2c, uint8_t volume, uint8_t balance);
HAL_StatusTypeDef pcm5242_mute(I2C_HandleTypeDef *hi2c);

void update_dsp(DSP_Manager *dsp, uint8_t *values);
Custom_Coeffs get_custom_coeffs(Coeffs coeffs);

#endif /* LIBS_PCM5242_PCM5242_H_ */
