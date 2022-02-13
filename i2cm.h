
#ifndef I2CM_H_
#define I2CM_H_

#include "sam.h"

/* Bus states */
#define I2CM_BUSSTATE_UNKNOWN 0
#define I2CM_BUSSTATE_IDLE    1
#define I2CM_BUSSTATE_OWNER   2
#define I2CM_BUSSTATE_BUSY    3

/* Master commands */
#define I2CM_CMD_NO_ACTION      0
#define I2CM_CMD_REPEATED_START 1
#define I2CM_CMD_BYTE_READ      2
#define I2CM_CMD_STOP_COND      3

/* Write/read flags */
#define I2CM_WRITE 0x00
#define I2CM_READ  0x01

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} i2cm_hw_t;

#define I2CM_HW_SERCOM0 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM0,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM0_CORE_Val	\
}

#define I2CM_HW_SERCOM1 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM1,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM1_CORE_Val	\
}

#define I2CM_HW_SERCOM2 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM2,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM2_CORE_Val	\
}

#define I2CM_HW_SERCOM3 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM3,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM3_CORE_Val	\
}

#define I2CM_HW_SERCOM4 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM4,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM4_CORE_Val	\
}

#define I2CM_HW_SERCOM5 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM5,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM5_CORE_Val	\
}

#define I2CM_SPEED_STANDARD_FAST_MODE 0 /* Standard (<100kHz) or Fast (<400kHz) Mode */
#define I2CM_SPEED_FAST_PLUS_MODE 1     /* Fast Mode Plus (<1MHz) */
#define I2CM_SPEED_HIGH_SPEED_MODE 2    /* High-Speed Mode (<3.4MHz) */

#define I2CM_SDAHOLD_DISABLED 0 
#define I2CM_SDAHOLD_75NS 1
#define I2CM_SDAHOLD_450NS 2
#define I2CM_SDAHOLD_600NS 3

typedef struct
{
	uint8_t speed;
    uint32_t baud;
	uint8_t sda_hold_time;
} i2cm_cfg_t;

#define I2CM_CFG_DEFAULT                    \
{ 						                    \
	.speed = I2CM_SPEED_STANDARD_FAST_MODE, \
	.baud= 100000,		                    \
	.sda_hold_time = I2CM_SDAHOLD_DISABLED  \
}

void i2cm_init(Sercom *SERCOMX, i2cm_hw_t *i2cm_hw, i2cm_cfg_t *i2cm_cfg);

int8_t i2cm_start(Sercom *SERCOMX, uint8_t address);
int8_t i2cm_write(Sercom *SERCOMX, const uint8_t *data, uint32_t len);
int8_t i2cm_write_single(Sercom *SERCOMX, uint8_t data);
void i2cm_stop(Sercom *SERCOMX);

#endif /* I2CM_H_ */

