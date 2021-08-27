
#include "clock.h"

void clock_1m_init()
{
	/* Set CLOCK_GCLK_1M_ID clock source to 8MHz internal oscillator, /8 prescaler (1MHz) */
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(CLOCK_GCLK_1M_ID) | GCLK_GENDIV_DIV(8);
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(CLOCK_GCLK_1M_ID) | GCLK_GENCTRL_SRC_OSC8M | GCLK_GENCTRL_GENEN;
	while (GCLK->STATUS.bit.SYNCBUSY);
}

void clock_8m_init()
{
	/* Enable 8MHz internal oscillator, disable prescaler */
	SYSCTRL->OSC8M.bit.ENABLE = 1;
	SYSCTRL->OSC8M.bit.PRESC = 0;

	/* Set GCLK0 clock source to 8MHz internal oscillator, disable prescaler */
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(0) | GCLK_GENDIV_DIV(1);
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0) | GCLK_GENCTRL_SRC_OSC8M | GCLK_GENCTRL_GENEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Update core frequency (this comes from GCLK0) */
	SystemCoreClock = 8000000UL;
}

void clock_48m_init()
{
	/* Direct GCLK0 (8MHz) to the DPLL */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(0) |
						GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_DFLL48_Val) |
						GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync before and after enabling DFLL */
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

	/* Write multiplier and step values */
	SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL(6) | SYSCTRL_DFLLMUL_CSTEP(31) | SYSCTRL_DFLLMUL_FSTEP(511);
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

	/* Write the coarse and fine calibration values stored in NVM */
	uint32_t coarse_cal = *(uint32_t *)FUSES_DFLL48M_COARSE_CAL_ADDR;
	coarse_cal = (coarse_cal & FUSES_DFLL48M_COARSE_CAL_Msk) >> FUSES_DFLL48M_COARSE_CAL_Pos;

	uint32_t fine_cal = *(uint32_t *)FUSES_DFLL48M_FINE_CAL_ADDR;
	fine_cal = (fine_cal & FUSES_DFLL48M_FINE_CAL_Msk) >> FUSES_DFLL48M_FINE_CAL_Pos;

	SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(coarse_cal) | SYSCTRL_DFLLVAL_FINE(fine_cal);

	/* Switch to closed-loop mode, enable waitlock */
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
	SYSCTRL->DFLLCTRL.reg |= (uint16_t)(SYSCTRL_DFLLCTRL_MODE | SYSCTRL_DFLLCTRL_WAITLOCK);

	/* Disable division for GCLK */
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(CLOCK_GCLK_48M_ID) | GCLK_GENDIV_DIV(1);

	/* Switch GCLK to the DFLL */
	GCLK->GENCTRL.reg =
		GCLK_GENCTRL_ID(CLOCK_GCLK_48M_ID) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M;
	while (GCLK->STATUS.bit.SYNCBUSY);
}

void clock_96m_init()
{
	/* Enable CLOCK_GCLK_1M_ID and connect it to DPLL GCLK source */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
						GCLK_CLKCTRL_GEN(CLOCK_GCLK_1M_ID) |
						GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_FDPLL_Val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Set FDPLL ratio to 1 MHz * (95 + 1) = 96 MHz */
	SYSCTRL->DPLLRATIO.reg = SYSCTRL_DPLLRATIO_LDR(95) | SYSCTRL_DPLLRATIO_LDRFRAC(0);

	/* Configure FDPLL to disregard phase lock and select GCLK_DPLL as source */
	SYSCTRL->DPLLCTRLB.reg = SYSCTRL_DPLLCTRLB_LBYPASS |
							 SYSCTRL_DPLLCTRLB_WUF |
							 SYSCTRL_DPLLCTRLB_REFCLK(SYSCTRL_DPLLCTRLB_REFCLK_GCLK_Val);

	/* Enable FDPLL */
	SYSCTRL->DPLLCTRLA.reg |= SYSCTRL_DPLLCTRLA_ENABLE;

	/* No prescaler for CLOCK_GCLK_96M_ID */
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(CLOCK_GCLK_96M_ID) | GCLK_GENDIV_DIV(1);

	/* Switch CLOCK_GCLK_96M_ID to the FDPLL */
	GCLK->GENCTRL.reg =
		GCLK_GENCTRL_ID(CLOCK_GCLK_96M_ID) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_FDPLL;
	while (GCLK->STATUS.bit.SYNCBUSY);
}