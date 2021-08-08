
#ifndef I2CM_H_
#define I2CM_H_

#include "sam.h"

/* Which hardware should be used for this I2CM instance? */
#define I2CM_SERCOM                     SERCOM2                          // Which SERCOM? (SERCOM0-2)
#define I2CM_SERCOM_APBCMASK            PM_APBCMASK_SERCOM2              // APB clock enable mask for this SERCOM
#define I2CM_SERCOM_CLKCTRL_ID_CORE_Val GCLK_CLKCTRL_ID_SERCOM2_CORE_Val // CLKCTRL ID for SERCOM peripheral (0x10 for SERCOM2)

/* I2C Master settings */
#define I2CM_BAUD 400000UL

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

void i2cm_init();

int8_t i2cm_start(uint8_t address);
int8_t i2cm_write(const uint8_t *data, uint32_t len);
int8_t i2cm_write_single(uint8_t data);
void i2cm_stop();

#endif /* I2CM_H_ */
