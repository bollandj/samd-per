/*
 * spim.h
 *
 * Created: 11/05/2021 22:04:28
 *  Author: Joshua
 */

#ifndef SPIM_H_
#define SPIM_H_

#include "sam.h"

void spim_init();
void spim_write(const uint8_t *data, uint32_t len);
void spim_write_single(uint8_t data);

#endif /* SPIM_H_ */