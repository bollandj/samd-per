#ifndef ADC_H
#define ADC_H

#include "sam.h"

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} adc_hw_t;

#define ADC_HW									\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_ADC,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_ADC_Val	\
}

typedef struct
{
	uint8_t refsel_val;
	uint8_t gain_val;
	uint8_t prescaler_val;
} adc_cfg_t;

#define ADC_CFG_DEFAULT								\
{ 													\
	.refsel_val = ADC_REFCTRL_REFSEL_INTVCC1_Val, 	\
	.gain_val = ADC_INPUTCTRL_GAIN_DIV2_Val, 		\
	.prescaler_val = ADC_CTRLB_PRESCALER_DIV256_Val \
}

void adc_init(Adc *ADCX, adc_hw_t *adc_hw, adc_cfg_t *adc_cfg);
uint16_t adc_read_single(Adc *ADCX, uint32_t mux_pos_val, uint32_t mux_neg_val);

#endif // ADC_H
