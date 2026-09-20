#include "pcm5242.h"

HAL_StatusTypeDef pcm5242_readRegister(I2C_HandleTypeDef *hi2c, uint8_t reg,
		uint8_t *value) {
	if ((hi2c == NULL) || (value == NULL)) {
		return HAL_ERROR;
	}

	/* Write du numero de registre, repeated start, puis read d'un octet. */
	return HAL_I2C_Mem_Read(hi2c, PCM5242_HAL_ADDR, reg, I2C_MEMADD_SIZE_8BIT,
			value, 1u, PCM5242_TIMEOUT);
}

HAL_StatusTypeDef pcm5242_writeRegister(I2C_HandleTypeDef *hi2c, uint8_t reg,
		uint8_t value) {
	if (hi2c == NULL) {
		return HAL_ERROR;
	}

	return HAL_I2C_Mem_Write(hi2c, PCM5242_HAL_ADDR, reg, I2C_MEMADD_SIZE_8BIT,
			&value, 1u, PCM5242_TIMEOUT);
}

/* Page utilisee pour l'aller-retour de test. Valeur non nulle volontaire :
 * un bus bloque a 0x00 (SDA tiree a la masse) ou a 0xFF (aucun composant,
 * seules les pull-ups repondent) ne peut pas la restituer.
 * Selectionner une page est sans effet de bord : cela ne fait que deplacer
 * la fenetre de registres, on n'ecrit rien dans la page 1 elle-meme. */
#define PCM5242_PROBE_PAGE  0x01u

bool pcm5242_checkCom(I2C_HandleTypeDef *hi2c) {
	uint8_t value = 0u;
	bool ok = false;
	if (hi2c == NULL)
		return false;
	if (HAL_I2C_IsDeviceReady(hi2c, PCM5242_HAL_ADDR, 3u, PCM5242_TIMEOUT)
			!= HAL_OK)
		return false;
	if ((pcm5242_writeRegister(hi2c, PCM5242_REG_PAGE, PCM5242_PROBE_PAGE)
			== HAL_OK)
			&& (pcm5242_readRegister(hi2c, PCM5242_REG_PAGE, &value) == HAL_OK)
			&& (value == PCM5242_PROBE_PAGE)) {
		ok = true;
	}
	if (pcm5242_writeRegister(hi2c, PCM5242_REG_PAGE, 0x00u) != HAL_OK)
		ok = false;
	if (!ok)
		return false;
	if (pcm5242_readRegister(hi2c, PCM5242_REG_PLL, &value) != HAL_OK)
		return false;
	return ((value & PCM5242_PLLE) != 0u);
}

void set_audio_level_and_balance(uint8_t gain, uint8_t balance){

}
