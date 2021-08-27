#ifndef PER_CFG_H_
#define PER_CFG_H_

#include "sam.h"

/**********************************************************************************************************************
 * Macros
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * GPIO
 **********************************************************************************************************************/

/* Pin assignments */
#define LED0_PORT PORT_PA09
#define LED0_PIN  PIN_PA09

#define LED1_PORT PORT_PA27
#define LED1_PIN  PIN_PA27

#define SW0_PORT PORT_PA28
#define SW0_PIN  PIN_PA28

/**********************************************************************************************************************
 * Systick
 **********************************************************************************************************************/

/* Systick settings */
#define SYSTICK_FREQ 1000UL

/**********************************************************************************************************************
 * DAC
 **********************************************************************************************************************/

/* Pin assignments */
#define DAC_VOUT_PIN      PIN_PA02
#define DAC_VOUT_PORT     (1 << DAC_VOUT_PIN)
#define DAC_VOUT_PMUX_Val PORT_PMUX_PMUXE_B_Val

/* DAC settings */
#define DAC_REFSEL_Val DAC_CTRLB_REFSEL_AVCC_Val

/**********************************************************************************************************************
 * SPI
 **********************************************************************************************************************/

/* Which hardware should be used for this SPI instance? */
#define SPI_SERCOM                     SERCOM0                          // Which SERCOM?
#define SPI_SERCOM_APBCMASK            PM_APBCMASK_SERCOM0              // APB clock enable mask for this SERCOM
#define SPI_SERCOM_CLKCTRL_ID_CORE_Val GCLK_CLKCTRL_ID_SERCOM0_CORE_Val // CLKCTRL ID for SERCOM peripheral

/* Pin assignments */
#define SPI_SCK_PIN      PIN_PA09
#define SPI_SCK_PORT     (1 << SPI_SCK_PIN) // PAD[3]
#define SPI_SCK_PMUX_Val PORT_PMUX_PMUXE_D_Val

#define SPI_SDO_PIN      PIN_PA06
#define SPI_SDO_PORT     (1 << SPI_SDO_PIN) // PAD[0]
#define SPI_SDO_PMUX_Val PORT_PMUX_PMUXE_C_Val

#define SPI_SDI_PIN      PIN_PA24
#define SPI_SDI_PORT     (1 << SPI_SDI_PIN)
#define SPI_SDI_PMUX_Val PORT_PMUX_PMUXE_C_Val

#define SPI_SS_PIN       PIN_PA23
#define SPI_SS_PORT      (1 << SPI_SS_PIN)
#define SPI_SS_PMUX_Val  PORT_PMUX_PMUXE_C_Val

/* SPI settings */
#define SPI_POLARITY 0
#define SPI_PHASE 0
#define SPI_BAUD 100000UL
#define SPI_GCLK_FREQ 8000000UL

/**********************************************************************************************************************
 * USART
 **********************************************************************************************************************/

/* Which hardware should be used for this USART instance? */
#define USART_SERCOM                     SERCOM1                          // Which SERCOM?
#define USART_SERCOM_APBCMASK            PM_APBCMASK_SERCOM1              // APB clock enable mask for this SERCOM
#define USART_SERCOM_CLKCTRL_ID_CORE_Val GCLK_CLKCTRL_ID_SERCOM1_CORE_Val // CLKCTRL ID for SERCOM peripheral

/* Interrupt handler */
#define USART_SERCOM_IRQn    SERCOM1_IRQn
#define USART_SERCOM_HANDLER SERCOM1_Handler

/* Pin assignments */
#define USART_TX_PIN      PIN_PA16C_SERCOM1_PAD2
#define USART_TX_PMUX_Val MUX_PA16C_SERCOM1_PAD2
#define USART_TXPO_Val    1 // PAD[2]

#define USART_RX_PIN      PIN_PA17C_SERCOM1_PAD3
#define USART_RX_PMUX_Val MUX_PA17C_SERCOM1_PAD3
#define USART_RXPO_Val    3 // PAD[3]

/* USART settings */
#define USART_BAUD 9600
#define USART_LSB_FIRST 1

/**********************************************************************************************************************
 * I2C Master
 **********************************************************************************************************************/

/* Which hardware should be used for this I2C master instance? */
#define I2CM_SERCOM                     SERCOM2                          // Which SERCOM? (SERCOM0-2)
#define I2CM_SERCOM_APBCMASK            PM_APBCMASK_SERCOM2              // APB clock enable mask for this SERCOM
#define I2CM_SERCOM_CLKCTRL_ID_CORE_Val GCLK_CLKCTRL_ID_SERCOM2_CORE_Val // CLKCTRL ID for SERCOM peripheral (0x10 for SERCOM2)

/* Pin assignments */
#define I2CM_SDA_PIN      PIN_PA14
#define I2CM_SDA_PORT     (1 << I2CM_SDA_PIN)
#define I2CM_SDA_PMUX_Val PORT_PMUX_PMUXE_D_Val

#define I2CM_SCL_PIN      PIN_PA15
#define I2CM_SCL_PORT     (1 << I2CM_SCL_PIN)
#define I2CM_SCL_PMUX_Val PORT_PMUX_PMUXE_D_Val

/* I2C master settings */
#define I2CM_BAUD 400000UL

/**********************************************************************************************************************
 * USB
 **********************************************************************************************************************/

/* Pin assignments */
#define USB_DM_PIN      PIN_PA24G_USB_DM
#define USB_DM_PMUX_Val MUX_PA24G_USB_DM

#define USB_DP_PIN      PIN_PA25G_USB_DP
#define USB_DP_PMUX_Val MUX_PA25G_USB_DP

#endif // PER_CFG_H_
