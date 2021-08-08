/*
 * spi.h
 *
 * Created: 11/05/2021 22:04:28
 *  Author: Joshua
 */

#ifndef SPI_H_
#define SPI_H_

#include "sam.h"

void spi_init();
void spi_write(const uint8_t *data, uint32_t len);
void spi_write_single(uint8_t data);

#endif /* SPI_H_ */