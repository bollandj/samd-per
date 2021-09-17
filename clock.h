
#ifndef CLOCK_H_
#define CLOCK_H_

#include "sam.h"

/* Clock source initialisation */
void clock_osc8m_init(); // Initialise 8MHz internal oscillator for CPU clock or general purpose
void clock_xosc_init(); // Initialise external crystal oscillator
void clock_dfll48m_init(uint8_t gclk_32k_id); // Initialise 48MHz DFLL using a 32kHz GCLK as source
void clock_dfll48m_usb_init(); // Initialise 48MHz DFLL using USB clock recovery
void clock_fdpll_init(uint8_t gclk_1m_id); // Initialise FDPLL at 96MHz using a 1MHz GCLK as source

/* Clock source connection to GCLKs */
void clock_osc8m_connect(uint8_t gclk_id, uint8_t prescaler);
void clock_xosc_connect(uint8_t gclk_id, uint8_t prescaler, uint32_t xosc_freq);
void clock_dfll48m_connect(uint8_t gclk_id, uint8_t prescaler);
void clock_fdpll_connect(uint8_t gclk_id, uint8_t prescaler);

#endif /* CLOCK_H_ */