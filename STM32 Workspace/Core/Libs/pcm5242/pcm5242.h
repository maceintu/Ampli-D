#ifndef LIBS_PCM5242_PCM5242_H_
#define LIBS_PCM5242_PCM5242_H_

#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include "pcm5242/biquad.h"

#define PCM5242_EQ_GAIN_MIN_DB       (-12.0)
#define PCM5242_EQ_GAIN_MAX_DB       ( 12.0)
#define PCM5242_EQ_GAIN_STEP_DB      (  0.5)
#define PCM5242_EQ_GAIN_LEVEL_COUNT  49u

/* Marge ajoutee de chaque cote des frontieres entre paliers.
 * Valeur de depart, a ajuster au bruit des mesures filtrees.
 */
#define PCM5242_EQ_HYSTERESIS_DB     (0.1)

/* Ecart au gain retenu necessaire pour changer de palier. */
#define PCM5242_EQ_CHANGE_THRESHOLD_DB (PCM5242_EQ_GAIN_STEP_DB / 2.0 + PCM5242_EQ_HYSTERESIS_DB)

// controle du volume
#define PCM5242_PAGE_0        0x00u   /* les registres de volume sont en page 0 */
#define PCM5242_PAGE_44        0x2Cu
#define PCM5242_PAGE_62        0x3Eu
#define PCM5242_REG_VOL_L    	0x3Du
#define PCM5242_REG_VOL_R    	0x3Eu
#define PCM5242_MIN_ATTENUATION 0x3Cu
#define PCM5242_MAX_ATTENUATION 0xFEu
#define PCM5242_VOL_MUTE 		0xFFu
#define PCM5242_BALANCE_RANGE 	80

#define PCM5242_CRAM_ADAPTIVE  0x04u  // Bit 2
#define PCM5242_CRAM_ACTIVE_B  0x02u  // Bit 1
#define PCM5242_CRAM_SWITCH    0x01u  // Bit 0

/* Adresse 7 bits du PCM5242 sur la carte Ampli-D (ADR1 et ADR2 a la masse). */
#define PCM5242_ADDR            0x4Cu

/* Le HAL attend l'adresse deja decalee d'un bit vers la gauche. */
#define PCM5242_HAL_ADDR        ((uint16_t)(PCM5242_ADDR << 1))

/* Delai d'attente I2C, en ms. */
#define PCM5242_TIMEOUT         50u

/* 0x00 - Page select. Present a l'offset 0 de toutes les pages. */
#define PCM5242_PAGE_UNKNOWN  	  0xFFu
#define PCM5242_REG_PAGE      	  0x00u

#define PCM5242_DSP_FILTER_NUMBER  3
#define PCM5242_BIQUAD_SCALE       8388608.0  // 2^23
#define PCM5242_BIQUAD_HALF_SCALE  4194304.0  // 2^22

typedef struct {
	float low;
	float medium;
	float high;
} Gains;

typedef struct {
	int32_t n0;
	int32_t n1;
	int32_t n2;
	int32_t d1;
	int32_t d2;
} Custom_Coeffs;

typedef struct {
	Shelf biquad_filter;
	Custom_Coeffs cutom_coeffs;
} PCM5242_Shelf_Filter;

typedef struct {
	Peaking biquad_filter;
	Custom_Coeffs cutom_coeffs;
} PCM5242_Peaking_Filter;

typedef struct {
	Gains gains;
	PCM5242_Shelf_Filter low_filter;
	PCM5242_Peaking_Filter medium_filter;
	PCM5242_Shelf_Filter high_filter;
} DSP_Manager;

HAL_StatusTypeDef pcm5242_read_register(I2C_HandleTypeDef *hi2c, uint8_t page, uint8_t reg, uint8_t *value);
HAL_StatusTypeDef pcm5242_write_register(I2C_HandleTypeDef *hi2c, uint8_t page, uint8_t reg, uint8_t value);

HAL_StatusTypeDef set_audio_level_and_balance(I2C_HandleTypeDef *hi2c, uint8_t volume, uint8_t balance);
HAL_StatusTypeDef pcm5242_mute(I2C_HandleTypeDef *hi2c);

void update_dsp(I2C_HandleTypeDef *hi2c, DSP_Manager *dsp, Gains gains);
Custom_Coeffs get_custom_coeffs(Coeffs coeffs);
HAL_StatusTypeDef write_custom_coeffs(I2C_HandleTypeDef *hi2c, Custom_Coeffs coeffs, uint8_t first_register, uint8_t page_number);

#endif /* LIBS_PCM5242_PCM5242_H_ */
