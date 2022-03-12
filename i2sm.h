
#ifndef I2SM_H_
#define I2SM_H_

#include "sam.h"

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} i2sm_hw_t;

#define I2SM_HW 							\
{											\
	.gclk_val = CLOCK_GCLK0,				\
	.apbcmask = PM_APBCMASK_I2S,			\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_I2S_0	\
}

typedef struct
{
	uint8_t datasize;
} i2sm_cfg_t;

#define I2SM_CFG_DEFAULT						\
{ 												\
	.datasize = I2SM_SERCTRL_DATASIZE_16_Val	\
}

void i2sm_init(I2s *I2SX, i2sm_hw_t *i2sm_hw, i2sm_cfg_t *i2sm_cfg);

#endif /* I2SM_H_ */