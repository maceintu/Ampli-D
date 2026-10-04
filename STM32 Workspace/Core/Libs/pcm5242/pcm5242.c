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
	HAL_StatusTypeDef ret_l = pcm5242_write_register(hi2c, PCM5242_PAGE_0,
	PCM5242_REG_VOL_L, (uint8_t) l_att);
	HAL_StatusTypeDef ret_r = pcm5242_write_register(hi2c, PCM5242_PAGE_0,
	PCM5242_REG_VOL_R, (uint8_t) r_att);
	return (ret_l != HAL_OK) ? ret_l : ret_r;
}

HAL_StatusTypeDef pcm5242_mute(I2C_HandleTypeDef *hi2c) {
	HAL_StatusTypeDef ret_l = pcm5242_write_register(hi2c, PCM5242_PAGE_0, PCM5242_REG_VOL_L, PCM5242_VOL_MUTE);
	HAL_StatusTypeDef ret_r = pcm5242_write_register(hi2c, PCM5242_PAGE_0, PCM5242_REG_VOL_R, PCM5242_VOL_MUTE);
	return (ret_l != HAL_OK) ? ret_l : ret_r;
}

void update_dsp(I2C_HandleTypeDef *hi2c, DSP_Manager *dsp, Gains gains) {
	uint8_t p44_reg1_val = 0;
	if (pcm5242_read_register(hi2c, PCM5242_PAGE_44, 1, &p44_reg1_val) != HAL_OK)
		return;
	if (p44_reg1_val & PCM5242_CRAM_SWITCH)
		return;

	bool has_to_switch = false;
	uint8_t write_page = (p44_reg1_val & PCM5242_CRAM_ACTIVE_B) ? PCM5242_PAGE_44 : PCM5242_PAGE_62;
	if (fabs(gains.low - dsp->gains.low) > PCM5242_EQ_CHANGE_THRESHOLD_DB) {
		dsp->gains.low = round(gains.low / PCM5242_EQ_GAIN_STEP_DB) * PCM5242_EQ_GAIN_STEP_DB;
		update_low_filter(&dsp->low_filter.biquad_filter, dsp->gains.low); // MAJ du biquad
		dsp->low_filter.cutom_coeffs = get_custom_coeffs(dsp->low_filter.biquad_filter.coeffs); // MAJ des coeffs PCM5242
		has_to_switch = true;
	}
	if (fabs(gains.medium - dsp->gains.medium) > PCM5242_EQ_CHANGE_THRESHOLD_DB) {
		dsp->gains.medium = round(gains.medium / PCM5242_EQ_GAIN_STEP_DB) * PCM5242_EQ_GAIN_STEP_DB;
		update_peaking_filter(&dsp->medium_filter.biquad_filter, dsp->gains.medium);
		dsp->medium_filter.cutom_coeffs = get_custom_coeffs(dsp->medium_filter.biquad_filter.coeffs);
		has_to_switch = true;
	}
	if (fabs(gains.high - dsp->gains.high) > PCM5242_EQ_CHANGE_THRESHOLD_DB) {
		dsp->gains.high = round(gains.high / PCM5242_EQ_GAIN_STEP_DB) * PCM5242_EQ_GAIN_STEP_DB;
		update_high_filter(&dsp->high_filter.biquad_filter, dsp->gains.high);
		dsp->high_filter.cutom_coeffs = get_custom_coeffs(dsp->high_filter.biquad_filter.coeffs);
		has_to_switch = true;
	}
	if (has_to_switch) {
		if (write_custom_coeffs(hi2c, dsp->low_filter.cutom_coeffs, 48, write_page) != HAL_OK)
			return;
		if (write_custom_coeffs(hi2c, dsp->medium_filter.cutom_coeffs, 68, write_page) != HAL_OK)
			return;
		if (write_custom_coeffs(hi2c, dsp->high_filter.cutom_coeffs, 88, write_page) != HAL_OK)
			return;
		pcm5242_write_register(hi2c, PCM5242_PAGE_44, 1, p44_reg1_val | PCM5242_CRAM_SWITCH); // switch le buffer
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

HAL_StatusTypeDef write_custom_coeffs(I2C_HandleTypeDef *hi2c, Custom_Coeffs coeffs, uint8_t first_register, uint8_t page_number) {
	if (page_number != PCM5242_PAGE_44 && page_number != PCM5242_PAGE_62)
		return HAL_ERROR;

	if (first_register != 48 && first_register != 68 && first_register != 88)
		return HAL_ERROR;

	int32_t values[5] = { coeffs.n0, coeffs.n1, coeffs.n2, coeffs.d1, coeffs.d2 };
	for (unsigned i = 0; i < 5; ++i) {
		if (values[i] < -8388608 || values[i] > 8388607)
			return HAL_ERROR;
	}
	for (unsigned i = 0; i < 5; ++i) {
		uint32_t value = (uint32_t) values[i];
		uint8_t bytes[3] = { (uint8_t) (value >> 16), (uint8_t) (value >> 8), (uint8_t) value };
		for (unsigned j = 0; j < 3; ++j) {
			uint8_t reg = (uint8_t) (first_register + 4u * i + j);
			HAL_StatusTypeDef ret = pcm5242_write_register(hi2c, page_number, reg, bytes[j]);
			if (ret != HAL_OK)
				return ret;
		}
	}
	return HAL_OK;
}
