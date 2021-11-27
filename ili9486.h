//STM32 ILI9486 8-bit TFT Library for LVGL
//Name: ili9486.h
//Author: Mike Kushnerik
//Date: 10/18/2021

#ifndef _ILI9486_H_
#define _ILI9486_H_

#include "lvgl.h"
#include "stm32f1xx_hal.h"
#include <stdlib.h>

/*****************************************************************************/
// TFT Definitions
/*****************************************************************************/

#define TFT_HOR_RES 480
#define TFT_VER_RES 320

/*****************************************************************************/
// Data and Control Port / Pin Definitions
/*****************************************************************************/

// Example: PB5-> PORT = GPIOB, MASK = GPIO_PIN_5

// Data and Control Ports

#define TFT_DATA_PORT	GPIOA
#define TFT_CNTRL_PORT	GPIOB

// Masks for Control Pins

#define TFT_RD_MASK		GPIO_PIN_9
#define TFT_WR_MASK		GPIO_PIN_8
#define TFT_RS_MASK		GPIO_PIN_7
#define TFT_CS_MASK		GPIO_PIN_6
#define TFT_RST_MASK	GPIO_PIN_5

/*****************************************************************************/
// TFT Register Names
/*****************************************************************************/

#define ILI9486_SOFTRESET         0x01
#define ILI9486_SLEEPIN           0x10
#define ILI9486_SLEEPOUT          0x11
#define ILI9486_NORMALDISP        0x13
#define ILI9486_INVERTOFF         0x20
#define ILI9486_INVERTON          0x21
#define ILI9486_GAMMASET          0x26
#define ILI9486_DISPLAYOFF        0x28
#define ILI9486_DISPLAYON         0x29
#define ILI9486_COLADDRSET        0x2A
#define ILI9486_PAGEADDRSET       0x2B
#define ILI9486_MEMORYWRITE       0x2C
#define ILI9486_PIXELFORMAT       0x3A
#define ILI9486_FRAMECONTROL      0xB1
#define ILI9486_DISPLAYFUNC       0xB6
#define ILI9486_ENTRYMODE         0xB7
#define ILI9486_POWERCONTROL1     0xC0
#define ILI9486_POWERCONTROL2     0xC1
#define ILI9486_VCOMCONTROL1      0xC5
#define ILI9486_VCOMCONTROL2      0xC7
#define ILI9486_MEMCONTROL        0x36
#define ILI9486_MADCTL			  0x36

#define ILI9486_MADCTL_MY  0x80
#define ILI9486_MADCTL_MX  0x40
#define ILI9486_MADCTL_MV  0x20
#define ILI9486_MADCTL_ML  0x10
#define ILI9486_MADCTL_RGB 0x00
#define ILI9486_MADCTL_BGR 0x08
#define ILI9486_MADCTL_MH  0x04

/*****************************************************************************/
// Macro Definitions
/*****************************************************************************/

#define RD_ACTIVE               { TFT_CNTRL_PORT->BRR  = TFT_RD_MASK; }
#define RD_IDLE                 { TFT_CNTRL_PORT->BSRR = TFT_RD_MASK; }

#define WR_ACTIVE				{ TFT_CNTRL_PORT->BRR  = TFT_WR_MASK; }
#define WR_IDLE					{ TFT_CNTRL_PORT->BSRR = TFT_WR_MASK; }
#define WR_STROBE 				{ WR_ACTIVE; WR_IDLE; }

#define CD_COMMAND				{ TFT_CNTRL_PORT->BRR  = TFT_RS_MASK; }
#define CD_DATA					{ TFT_CNTRL_PORT->BSRR = TFT_RS_MASK; }

#define CS_ACTIVE				{ TFT_CNTRL_PORT->BRR  = TFT_CS_MASK; }
#define CS_IDLE					{ TFT_CNTRL_PORT->BSRR = TFT_CS_MASK; }

#define RST_ACTIVE				{ TFT_CNTRL_PORT->BRR = TFT_RST_MASK; }
#define RST_IDLE				{ TFT_CNTRL_PORT->BSRR = TFT_RST_MASK; }
#define RST_TOGGLE				{ RST_IDLE; RST_ACTIVE; RST_IDLE; }

#define writeByte(c) { TFT_DATA_PORT->BSRR = (uint32_t)(0x00FF0000 + ((c)&0xFF)); WR_STROBE; }

/*****************************************************************************/
// Function Prototypes
/*****************************************************************************/

void tft_init(void);
void ili9486_init(void);

void pushColors(uint16_t *data, int16_t len, bool first);
void drawPixel(int16_t x, int16_t y, uint16_t color);

void writeCommand(uint8_t c);
void writeData(uint8_t c);

#endif
