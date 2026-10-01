#include "pcm5242.h"
#include "math.h"

static uint8_t current_page = PCM5242_PAGE_UNKNOWN;

static HAL_StatusTypeDef pcm5242_select_page(I2C_HandleTypeDef *hi2c, uint8_t page) {
	if (page == current_page)
		return HAL_OK;
	HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(hi2c, PCM5242_HAL_ADDR, PCM5242_REG_PAGE,
	I2C_MEMADD_SIZE_8BIT, &page, 1u, PCM5242_TIMEOUT);
	current_page = (ret == HAL_OK) ? page : PCM5242_PAGE_UNKNOWN;
	return ret;
}

HAL_StatusTypeDef pcm5242_read_register(I2C_HandleTypeDef *hi2c, uint8_t page, uint8_t reg, uint8_t *value) {
	HAL_StatusTypeDef ret = pcm5242_select_page(hi2c, page);
	if (ret != HAL_OK)
		return ret;
	return HAL_I2C_Mem_Read(hi2c, PCM5242_HAL_ADDR, reg, I2C_MEMADD_SIZE_8BIT, value, 1u, PCM5242_TIMEOUT);
}

HAL_StatusTypeDef pcm5242_write_register(I2C_HandleTypeDef *hi2c, uint8_t page, uint8_t reg, uint8_t value) {
	HAL_StatusTypeDef ret = pcm5242_select_page(hi2c, page);
	if (ret != HAL_OK)
		return ret;
	return HAL_I2C_Mem_Write(hi2c, PCM5242_HAL_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &value, 1u, PCM5242_TIMEOUT);
}

HAL_StatusTypeDef set_audio_level_and_balance(I2C_HandleTypeDef *hi2c, uint8_t volume, uint8_t balance) {

	uint8_t attenuation = PCM5242_MIN_ATTENUATION + (255u - volume) * (PCM5242_MAX_ATTENUATION - PCM5242_MIN_ATTENUATION) / 255;
	uint16_t r_att = attenuation;
	uint16_t l_att = attenuation;
	if (balance >= 128) // R
			{
		l_att += (balance - 128u) * PCM5242_BALANCE_RANGE / 127u;
		if (l_att > PCM5242_VOL_MUTE)
			l_att = PCM5242_VOL_MUTE;
	} else {
		r_att += (128u - balance) * PCM5242_BALANCE_RANGE / 128u;
		if (r_att > PCM5242_VOL_MUTE)
			r_att = PCM5242_VOL_MUTE;
	}
	HAL_StatusTypeDef ret_l = pcm5242_write_register(hi2c, PCM5242_VOL_PAGE,
	PCM5242_REG_VOL_L, (uint8_t) l_att);
	HAL_StatusTypeDef ret_r = pcm5242_write_register(hi2c, PCM5242_VOL_PAGE,
	PCM5242_REG_VOL_R, (uint8_t) r_att);
	return (ret_l != HAL_OK) ? ret_l : ret_r;
}

HAL_StatusTypeDef pcm5242_mute(I2C_HandleTypeDef *hi2c) {
	HAL_StatusTypeDef ret_l = pcm5242_write_register(hi2c, PCM5242_VOL_PAGE, PCM5242_REG_VOL_L, PCM5242_VOL_MUTE);
	HAL_StatusTypeDef ret_r = pcm5242_write_register(hi2c, PCM5242_VOL_PAGE, PCM5242_REG_VOL_R, PCM5242_VOL_MUTE);
	return (ret_l != HAL_OK) ? ret_l : ret_r;
}

void update_dsp(DSP_Manager *dsp, Gains gains) {
	if (fabs(gains.low - dsp->gains.low) > PCM5242_EQ_CHANGE_THRESHOLD_DB) {
		dsp->gains.low = round(gains.low / PCM5242_EQ_GAIN_STEP_DB) * PCM5242_EQ_GAIN_STEP_DB;
		update_low_filter(&dsp->low_filter, dsp->gains.low);
	}
	if (fabs(gains.medium - dsp->gains.medium) > PCM5242_EQ_CHANGE_THRESHOLD_DB) {
		dsp->gains.medium = round(gains.medium / PCM5242_EQ_GAIN_STEP_DB) * PCM5242_EQ_GAIN_STEP_DB;
		update_peaking_filter(&dsp->medium_filter, dsp->gains.medium);
	}
	if (fabs(gains.high - dsp->gains.high) > PCM5242_EQ_CHANGE_THRESHOLD_DB) {
		dsp->gains.high = round(gains.high / PCM5242_EQ_GAIN_STEP_DB) * PCM5242_EQ_GAIN_STEP_DB;
		update_high_filter(&dsp->high_filter, dsp->gains.high);
	}
}

Custom_Coeffs get_custom_coeffs(Coeffs coeffs) {
	Custom_Coeffs result;
	result.n0 = (int32_t) round(coeffs.b0 * PCM5242_BIQUAD_SCALE);
	result.n1 = (int32_t) round(coeffs.b1 * PCM5242_BIQUAD_HALF_SCALE);
	result.n2 = (int32_t) round(coeffs.b2 * PCM5242_BIQUAD_SCALE);
	result.d1 = (int32_t) round(-coeffs.a1 * PCM5242_BIQUAD_HALF_SCALE);
	result.d2 = (int32_t) round(-coeffs.a2 * PCM5242_BIQUAD_SCALE);
	return result;
}

HAL_StatusTypeDef write_custom_coeffs(Custom_Coeffs coeffs) {
}
