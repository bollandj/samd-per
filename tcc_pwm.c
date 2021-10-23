/*
 * tcc_pwm.c
 *
 * Created: 12/05/2021 17:48:05
 *  Author: Joshua
 */

#include "tcc_pwm.h"

void tcc_pwm_init(Tcc *TCCX, tcc_pwm_hw_t *tcc_pwm_hw, tcc_pwm_cfg_t *tcc_pwm_cfg)
{
	/* Enable the APB clock for TCC */
	PM->APBCMASK.reg |= tcc_pwm_hw->apbcmask;

	/* Clock TCC with selected GCLK */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(tcc_pwm_hw->clkctrl_id_val)
					| GCLK_CLKCTRL_CLKEN
					| GCLK_CLKCTRL_GEN(tcc_pwm_hw->gclk_val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Disable TCC */
	while (TCCX->SYNCBUSY.bit.ENABLE);
	TCCX->CTRLA.bit.ENABLE = 0;

	/* Reset TCC */
	while (TCCX->CTRLA.bit.SWRST);
	TCCX->CTRLA.bit.SWRST = 1;
	while (TCCX->SYNCBUSY.bit.SWRST || TCCX->CTRLA.bit.SWRST);

	/* Set prescaler */
	TCCX->CTRLA.bit.PRESCALER = tcc_pwm_cfg->prescaler_val;

	/* Set period */
	TCCX->PER.reg = tcc_pwm_cfg->period;

	/* Use Normal PWM */
	TCCX->WAVE.bit.WAVEGEN = TCC_WAVE_WAVEGEN_NPWM_Val;
	while (TCCX->SYNCBUSY.bit.WAVE);

	/* Enable dead time generation */
	//TCCX->WEXCTRL.reg = TCC_WEXCTRL_DTHS(8)
	//					| TCC_WEXCTRL_DTLS(8)
	//					| TCC_WEXCTRL_DTIEN0;

	/* Enable output */
	TCCX->CTRLA.bit.ENABLE = 1;
	while (TCCX->SYNCBUSY.bit.ENABLE);
}
