#ifndef BIQUAD_H
#define BIQUAD_H

#define DSP_INPUT_FREQ 48000.0

/* TODO : gérer la conversion des coefficients au format PCM5242.
 * +1.0 correspond à 8388608, hors plage signée 24 bits
 * (maximum : 8388607). Ne pas l'encoder directement en 0x800000
 * car cette valeur représente -1.0. Vérifier aussi la neutralité
 * à 0 dB après quantification.
 */

typedef struct {
	double b0;
	double b1;
	double b2;
	double a1;
	double a2;
} Coeffs;

typedef struct {
	Coeffs coeffs;
	double alpha;
	double cos_w0;
} Shelf;

typedef struct {
	Coeffs coeffs;
	double alpha;
	double cos_w0;
} Peaking;

void init_shelf_filter(Shelf *filter, double frequency_hz, double sample_rate_hz);
void init_peaking_filter(Peaking *filter, double frequency_hz, double sample_rate_hz, double q);

void update_low_filter(Shelf *filter, double gain);
void update_high_filter(Shelf *filter, double gain);
void update_peaking_filter(Peaking *filter, double gain);

#endif
