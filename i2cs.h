
#ifndef I2CS_H_
#define I2CS_H_

#include "sam.h"

/* Bus states */
#define I2CS_BUSSTATE_UNKNOWN 0
#define I2CS_BUSSTATE_IDLE    1
#define I2CS_BUSSTATE_OWNER   2
#define I2CS_BUSSTATE_BUSY    3

/* Master commands */
#define I2CS_CMD_NO_ACTION      0
#define I2CS_CMD_REPEATED_START 1
#define I2CS_CMD_BYTE_READ      2
#define I2CS_CMD_STOP_COND      3

/* Write/read flags */
#define I2CS_WRITE 0x00
#define I2CS_READ  0x01

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} i2cs_hw_t;

#define I2CS_HW_SERCOM0 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM0,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM0_CORE_Val	\
}

#define I2CS_HW_SERCOM1 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM1,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM1_CORE_Val	\
}

#define I2CS_HW_SERCOM2 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM2,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM2_CORE_Val	\
}

#define I2CS_HW_SERCOM3 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM3,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM3_CORE_Val	\
}

#define I2CS_HW_SERCOM4 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM4,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM4_CORE_Val	\
}

#define I2CS_HW_SERCOM5 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM5,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM5_CORE_Val	\
}

void i2cs_init(Sercom *SERCOMX, i2cs_hw_t *i2cs_hw, uint8_t i2cs_addr);

void i2cs_enable(IRQn_Type SERCOMX_IRQn);
void i2cs_disable(IRQn_Type SERCOMX_IRQn);


#endif /* I2CS_H_ */

