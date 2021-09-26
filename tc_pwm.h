

#ifndef TC_PWM_H_
#define TC_PWM_H_

#include "sam.h"
#include "clock.h"
#include "gpio.h"

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} tc_pwm_hw_t;

#define TC_PWM_HW_TC3 							\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_TC3,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_TCC2_TC3	\
}

#define TC_PWM_HW_TC4 							\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_TC4,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_TC4_TC5	\
}

#define TC_PWM_HW_TC5 							\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_TC5,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_TC4_TC5	\
}

#define TC_PWM_HW_TC6 							\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_TC6,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_TC6_TC7	\
}

#define TC_PWM_HW_TC7 							\
{												\
	.gclk_val = CLOCK_GCLK0,					\
	.apbcmask = PM_APBCMASK_TC7,				\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_TC6_TC7	\
}

typedef struct
{
	uint32_t period;
	uint8_t prescaler_val;
} tc_pwm_cfg_t;

#define TC_PWM_CFG_DEFAULT 							\
{ 													\
	.period = 255,          						\
	.prescaler_val = TC_CTRLA_PRESCALER_DIV1_Val	\
}

void tc_pwm_init(Tc *TCX, tc_pwm_hw_t *tc_pwm_hw, tc_pwm_cfg_t *tc_pwm_cfg);
inline void tc_pwm_set_duty(Tc *TCX, uint8_t channel, uint8_t duty) { TCX->COUNT8.CC[channel].reg = duty; }
inline void tc_pwm_set_period(Tc *TCX, uint32_t period) { TCX->COUNT8.PER.reg = period;}
inline void tc_pwm_set_prescaler(Tc *TCX, uint8_t prescaler_val) { TCX->COUNT8.CTRLA.bit.PRESCALER = prescaler_val; }

#endif /* TC_PWM_H_ */