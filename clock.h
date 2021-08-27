
#ifndef CLOCK_H_
#define CLOCK_H_

#include "sam.h"

#define CLOCK_GCLK_1M_ID  1
#define CLOCK_GCLK_48M_ID 2
#define CLOCK_GCLK_96M_ID 3

void clock_1m_init();
void clock_8m_init();
void clock_48m_init();
void clock_96m_init();

#endif /* CLOCK_H_ */