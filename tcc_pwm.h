/*
 * tcc_pwm.h
 *
 * Created: 12/05/2021 17:48:14
 *  Author: Joshua
 */

#ifndef TCC_PWM_H_
#define TCC_PWM_H_

#include "sam.h"
#include "clock.h"

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} tcc_pwm_hw_t;

#define TCC_PWM_HW_TCC0 						\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_TCC0,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_TCC0_TCC1	\
}

#define TCC_PWM_HW_TCC1 						\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_TCC1,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_TCC0_TCC1 \
}

#define TCC_PWM_HW_TCC2							\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_TCC2,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_TCC2_TC3	\
}

typedef struct
{
	uint32_t period;
	uint8_t prescaler_val;
} tcc_pwm_cfg_t;

#define TCC_PWM_CFG_DEFAULT 						\
{ 													\
	.period = 255,          						\
	.prescaler_val = TCC_CTRLA_PRESCALER_DIV1_Val	\
}

void tcc_pwm_init(Tcc *TCCX, tcc_pwm_hw_t *tcc_pwm_hw, tcc_pwm_cfg_t *tcc_pwm_cfg);
inline void tcc_pwm_set_duty(Tcc *TCCX, uint8_t channel, uint32_t duty) { TCCX->CCB[channel].reg = duty; }
inline void tcc_pwm_set_period(Tcc *TCCX, uint32_t period) { TCCX->PERB.reg = period; }
inline void tcc_pwm_set_prescaler(Tcc *TCCX, uint8_t prescaler_val) { TCCX->CTRLA.bit.PRESCALER = prescaler_val; }

#endif /* TCC_PWM_H_ */