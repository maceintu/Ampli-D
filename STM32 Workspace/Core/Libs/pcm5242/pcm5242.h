/*
 * pcm5242.h
 *
 *  Created on: 20 sept. 2026
 *      Author: preel
 */

#ifndef LIBS_PCM5242_PCM5242_H_
#define LIBS_PCM5242_PCM5242_H_

#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/* Adresse 7 bits du PCM5242 sur la carte Ampli-D (ADR1 et ADR2 a la masse). */
#define PCM5242_ADDR            0x4Cu

/* Le HAL attend l'adresse deja decalee d'un bit vers la gauche. */
#define PCM5242_HAL_ADDR        ((uint16_t)(PCM5242_ADDR << 1))

/* Delai d'attente I2C, en ms. */
#define PCM5242_TIMEOUT         50u

/* 0x00 - Page select. Present a l'offset 0 de toutes les pages. */
#define PCM5242_REG_PAGE        0x00u

/* 0x01 - Reset. Bits auto-effaces, a n'ecrire qu'en standby. */
#define PCM5242_REG_RESET       0x01u
#define PCM5242_RSTM            (1u << 4)   /* reset des modules, vide la CRAM du DSP */
#define PCM5242_RSTR            (1u << 0)   /* reset des registres de mode */

/* 0x02 - Standby / powerdown. */
#define PCM5242_REG_STANDBY     0x02u
#define PCM5242_RQST            (1u << 4)   /* 1 = standby */
#define PCM5242_RQPD            (1u << 0)   /* 1 = powerdown, prioritaire sur standby */

/* 0x03 - Soft mute par canal, avec rampe (pas de pop). */
#define PCM5242_REG_MUTE        0x03u
#define PCM5242_RQML            (1u << 4)   /* 1 = mute canal gauche */
#define PCM5242_RQMR            (1u << 0)   /* 1 = mute canal droit */

/* 0x04 - PLL. PLLE vaut 1 au reset : bon registre pour un premier read de controle. */
#define PCM5242_REG_PLL         0x04u
#define PCM5242_PLCK            (1u << 4)   /* lecture seule. ATTENTION : 0 = verrouillee, 1 = PAS verrouillee */
#define PCM5242_PLLE            (1u << 0)   /* 1 = PLL interne activee (defaut) */


HAL_StatusTypeDef pcm5242_readRegister(I2C_HandleTypeDef *hi2c,
                                       uint8_t reg,
                                       uint8_t *value);

HAL_StatusTypeDef pcm5242_writeRegister(I2C_HandleTypeDef *hi2c,
                                        uint8_t reg,
                                        uint8_t value);


bool pcm5242_checkCom(I2C_HandleTypeDef *hi2c);

void set_audio_level_and_balance(uint8_t gain, uint8_t balance);

#endif /* LIBS_PCM5242_PCM5242_H_ */
