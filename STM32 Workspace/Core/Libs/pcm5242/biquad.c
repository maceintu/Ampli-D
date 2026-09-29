#include "pcm5242/biquad.h"
#include <math.h>

static const double PI = 3.14159265358979323846;

void init_shelf_filter(Shelf *filter, double frequency_hz, double sample_rate_hz) {
	// Ici on considère slope à 1, peutetre le passer en paramètre
	filter->coeffs = (Coeffs) { .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a1 = 0.0, .a2 = 0.0 };
	const double omega = 2.0 * PI * frequency_hz / sample_rate_hz;
	filter->cos_w0 = cos(omega);
	filter->alpha = sin(omega) / sqrt(2.0);
}

void init_peaking_filter(Peaking *filter, double frequency_hz, double sample_rate_hz, double q) {
	filter->coeffs = (Coeffs ) { .b0 = 1.0, .b1 = 0.0, .b2 = 0.0, .a1 = 0.0, .a2 = 0.0 };
	const double omega = 2.0 * PI * frequency_hz / sample_rate_hz;
	filter->cos_w0 = cos(omega);
	filter->alpha = sin(omega) / (2.0 * q);
}

void update_low_filter(Shelf *filter, double gain) {
	const double A = pow(10.0, gain / 40.0);
	const double beta = 2.0 * sqrt(A) * filter->alpha;
	const double a0 = (A + 1.0) + (A - 1.0) * filter->cos_w0 + beta;
	filter->coeffs.b0 = A * ((A + 1.0) - (A - 1.0) * filter->cos_w0 + beta) / a0;
	filter->coeffs.b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * filter->cos_w0) / a0;
	filter->coeffs.b2 = A * ((A + 1.0) - (A - 1.0) * filter->cos_w0 - beta) / a0;
	filter->coeffs.a1 = -2.0 * ((A - 1.0) + (A + 1.0) * filter->cos_w0) / a0;
	filter->coeffs.a2 = ((A + 1.0) + (A - 1.0) * filter->cos_w0 - beta) / a0;
}

void update_high_filter(Shelf *filter, double gain) {
	const double A = pow(10.0, gain / 40.0);
	const double beta = 2.0 * sqrt(A) * filter->alpha;
	const double a0 = (A + 1.0) - (A - 1.0) * filter->cos_w0 + beta;
	filter->coeffs.b0 = A * ((A + 1.0) + (A - 1.0) * filter->cos_w0 + beta) / a0;
	filter->coeffs.b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * filter->cos_w0) / a0;
	filter->coeffs.b2 = A * ((A + 1.0) + (A - 1.0) * filter->cos_w0 - beta) / a0;
	filter->coeffs.a1 = 2.0 * ((A - 1.0) - (A + 1.0) * filter->cos_w0) / a0;
	filter->coeffs.a2 = ((A + 1.0) - (A - 1.0) * filter->cos_w0 - beta) / a0;
}

void update_peaking_filter(Peaking *filter, double gain) {
	const double A = pow(10.0, gain / 40.0);
	const double alpha_mul_A = filter->alpha * A;
	const double alpha_div_A = filter->alpha / A;
	const double a0 = 1.0 + alpha_div_A;
	filter->coeffs.b0 = (1 + alpha_mul_A) / a0;
	filter->coeffs.b1 = (-2 * filter->cos_w0) / a0;
	filter->coeffs.b2 = (1 - alpha_mul_A) / a0;
	filter->coeffs.a1 = filter->coeffs.b1;
	filter->coeffs.a2 = (1 - alpha_div_A) / a0;
}
