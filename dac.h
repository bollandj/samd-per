#ifndef DAC_H
#define DAC_H

#include "sam.h"
#include "clock.h"

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} dac_hw_t;

#define DAC_HW									\
{												\
	.gclk_val = CLOCK_GCLK1,					\
	.apbcmask = PM_APBCMASK_DAC,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_DAC_Val	\
}

typedef struct
{
	uint8_t leftadj;
	uint8_t refsel;
} dac_cfg_t;

#define DAC_CFG_DEFAULT					\
{ 										\
	.leftadj = 0, 						\
	.refsel = DAC_CTRLB_REFSEL_AVCC_Val	\
}

void dac_init(Dac *DACX, dac_hw_t *dac_hw, dac_cfg_t *dac_cfg);
void dac_write(Dac *DACX, uint16_t val);

#endif // DAC_H
