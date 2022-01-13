
#include "tc_pwm.h"

void tc_pwm_init(Tc *TCX, tc_pwm_hw_t *tc_pwm_hw, tc_pwm_cfg_t *tc_pwm_cfg)
{
	/* Enable the APB clock for TC */
	PM->APBCMASK.reg |= tc_pwm_hw->apbcmask;

	/* Clock TC with selected GCLK */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(tc_pwm_hw->clkctrl_id_val)
					| GCLK_CLKCTRL_CLKEN
					| GCLK_CLKCTRL_GEN(tc_pwm_hw->gclk_val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Disable TC */
	while (TCX->COUNT8.STATUS.bit.SYNCBUSY);
	TCX->COUNT8.CTRLA.bit.ENABLE = 0;

	/* Reset TC */
	while (TCX->COUNT8.STATUS.bit.SYNCBUSY);
	TCX->COUNT8.CTRLA.bit.SWRST = 1;
	while (TCX->COUNT8.CTRLA.bit.SWRST || TCX->COUNT8.STATUS.bit.SYNCBUSY);

	/* Configure Count Mode (8-bit) */
	TCX->COUNT8.CTRLA.bit.MODE = TC_CTRLA_MODE_COUNT8_Val;

	/* Set prescaler */
	TCX->COUNT8.CTRLA.bit.PRESCALER = tc_pwm_cfg->prescaler_val;

	/* Set period */
	TCX->COUNT8.PER.reg = tc_pwm_cfg->period;

	/* Use Normal PWM */
	TCX->COUNT8.CTRLA.bit.WAVEGEN = TC_CTRLA_WAVEGEN_NPWM_Val;

	/* Enable TC */
	TCX->COUNT8.CTRLA.bit.ENABLE = 1;
	while (TCX->COUNT8.STATUS.bit.SYNCBUSY);
}
