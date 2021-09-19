
#include "clock.h"

void clock_osc8m_init()
{
	/* Enable 8MHz internal oscillator, disable prescaler */
	SYSCTRL->OSC8M.reg = SYSCTRL_OSC8M_ENABLE | SYSCTRL_OSC8M_PRESC(0);
	while (!SYSCTRL->PCLKSR.bit.OSC8MRDY);
}

void clock_xosc_init()
{
	/* Enable 8MHz internal oscillator, 62.5ms startup time, 2-pin crystal connection */
	SYSCTRL->XOSC.reg = SYSCTRL_XOSC_ENABLE | SYSCTRL_XOSC_STARTUP(0x0B) | SYSCTRL_XOSC_XTALEN;

	/* Wait until crystal oscillator has settled */
	while (!SYSCTRL->PCLKSR.bit.XOSCRDY);

	/* Enable automatic amplitude gain control */
	SYSCTRL->XOSC.bit.AMPGC = 1;

	while (!SYSCTRL->PCLKSR.bit.XOSCRDY);
}

void clock_dfll48m_init(uint8_t gclk_32k_id)
{
	/* Configure the DFLL to generate a 48MHz clock derived from another GCLK */
	/* The DFLL will operate in closed-loop mode */

	/* Direct 32.768kHz GCLK to be used as the DFLL reference */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
						GCLK_CLKCTRL_GEN(gclk_32k_id) |
						GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_DFLL48_Val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* See SAMD21 errata item 1.2.1 */
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

	/* Write the coarse and fine calibration values stored in NVM */
	uint32_t coarse_cal = *(uint32_t *)FUSES_DFLL48M_COARSE_CAL_ADDR;
	coarse_cal = (coarse_cal & FUSES_DFLL48M_COARSE_CAL_Msk) >> FUSES_DFLL48M_COARSE_CAL_Pos;

	uint32_t fine_cal = *(uint32_t *)FUSES_DFLL48M_FINE_CAL_ADDR;
	fine_cal = (fine_cal & FUSES_DFLL48M_FINE_CAL_Msk) >> FUSES_DFLL48M_FINE_CAL_Pos;

	SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(coarse_cal) | SYSCTRL_DFLLVAL_FINE(fine_cal);
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

	/* Disable chill cycle */
	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_CCDIS;

	/* Set multiplier (48M/32.768k = 1465 */
	SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL(1465) | SYSCTRL_DFLLMUL_CSTEP(31) | SYSCTRL_DFLLMUL_FSTEP(511);

	/* Closed-loop mode */
	SYSCTRL->DFLLCTRL.bit.MODE = 1;

	/* Enable the DFLL */
	SYSCTRL->DFLLCTRL.bit.ENABLE = 1;
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
}

void clock_dfll48m_usb_init()
{
	/* Configure the DFLL to generate a 48MHz clock derived from recovered USB clock */
	/* The DFLL will operate in closed-loop when connected to a USB host and open-loop otherwise */

	/* This is needed as per the errata - accessing the DPLL before doing this can lock the processor. */
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE;
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

	/* Write the coarse and fine calibration values stored in NVM */
	uint32_t coarse_cal = *(uint32_t *)FUSES_DFLL48M_COARSE_CAL_ADDR;
	coarse_cal = (coarse_cal & FUSES_DFLL48M_COARSE_CAL_Msk) >> FUSES_DFLL48M_COARSE_CAL_Pos;

	uint32_t fine_cal = *(uint32_t *)FUSES_DFLL48M_FINE_CAL_ADDR;
	fine_cal = (fine_cal & FUSES_DFLL48M_FINE_CAL_Msk) >> FUSES_DFLL48M_FINE_CAL_Pos;

	SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(coarse_cal) | SYSCTRL_DFLLVAL_FINE(fine_cal);
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

	/* Enable USB clock recovery mode. */
	SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_USBCRM | SYSCTRL_DFLLCTRL_CCDIS;

	/*
        From datasheet section 37.15, Note 1: When using DFLL48M in USB recovery mode,
        the Fine Step value must be Ah to guarantee a USB clock at
        +/-0.25% before 11ms after a resume.
    */
	SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL(48000) | SYSCTRL_DFLLMUL_CSTEP(1) | SYSCTRL_DFLLMUL_FSTEP(0xA);

	/* Setting to closed loop mode with USBCRM means that
       the DFLL will operate in closed loop when USB is
       connected, but will otherwise operate in open
       loop mode */
	SYSCTRL->DFLLCTRL.bit.MODE = 1;

	/* Enable the DFLL */
	SYSCTRL->DFLLCTRL.bit.ENABLE = 1;
	while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
}

void clock_fdpll_init(uint8_t gclk_1m_id)
{
	/* Direct 1MHZ GCLK to the FDPLL */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
						GCLK_CLKCTRL_GEN(gclk_1m_id) |
						GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_FDPLL_Val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Set FDPLL ratio to 1 MHz * (95 + 1) = 96MHz */
	SYSCTRL->DPLLRATIO.reg = SYSCTRL_DPLLRATIO_LDR(95) | SYSCTRL_DPLLRATIO_LDRFRAC(0);

	/* Configure FDPLL to disregard phase lock and select GCLK_DPLL as source */
	SYSCTRL->DPLLCTRLB.reg = SYSCTRL_DPLLCTRLB_LBYPASS |
							SYSCTRL_DPLLCTRLB_WUF |
							SYSCTRL_DPLLCTRLB_REFCLK(SYSCTRL_DPLLCTRLB_REFCLK_GCLK_Val);

	/* Enable FDPLL */
	SYSCTRL->DPLLCTRLA.reg |= SYSCTRL_DPLLCTRLA_ENABLE;
}

void clock_osc8m_connect(uint8_t gclk_id, uint8_t prescaler)
{
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(gclk_id) | GCLK_GENDIV_DIV(prescaler);
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(gclk_id) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC8M;
	while (GCLK->STATUS.bit.SYNCBUSY);

	if (gclk_id == 0) SystemCoreClock = 8000000UL/prescaler;
}

void clock_xosc_connect(uint8_t gclk_id, uint8_t prescaler, uint32_t xosc_freq)
{
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(gclk_id) | GCLK_GENDIV_DIV(prescaler);
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(gclk_id) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC;
	while (GCLK->STATUS.bit.SYNCBUSY);

	if (gclk_id == 0) SystemCoreClock = xosc_freq/prescaler;
}

void clock_dfll48m_connect(uint8_t gclk_id, uint8_t prescaler)
{
	if (gclk_id == 0 && prescaler == 1)
	{
		/* As we are operating above 24MHz core clock, 1 flash wait state is required at 3.3V */
		/* (see table 37-42 in SAMD21 datasheet) */
		NVMCTRL->CTRLB.bit.RWS = 1;
	}

	GCLK->GENDIV.reg = GCLK_GENDIV_ID(gclk_id) | GCLK_GENDIV_DIV(prescaler);
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(gclk_id) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M;
	while (GCLK->STATUS.bit.SYNCBUSY);

	if (gclk_id == 0) SystemCoreClock = 48000000UL/prescaler;
}

void clock_fdpll_connect(uint8_t gclk_id, uint8_t prescaler)
{
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(gclk_id) | GCLK_GENDIV_DIV(prescaler);
	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(gclk_id) | GCLK_GENCTRL_IDC | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_FDPLL;
	while (GCLK->STATUS.bit.SYNCBUSY);

	if (gclk_id == 0) SystemCoreClock = 96000000UL/prescaler; // note max. core clock is 48MHz
}