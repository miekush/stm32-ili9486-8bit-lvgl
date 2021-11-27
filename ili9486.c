//STM32 ILI9486 8-bit TFT Library for LVGL
//Name: ili9486.c
//Author: Mike Kushnerik
//Date: 10/18/2021

#include "ili9486.h"

static void tft_flush(lv_disp_drv_t * data_regv, const lv_area_t * area, lv_color_t * color_p);
static void setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

lv_disp_drv_t disp_data_regv;

/*****************************************************************************/

void tft_init(void)
{
	ili9486_init();

	static lv_disp_draw_buf_t disp_buf;
	static lv_color_t buf_1[TFT_HOR_RES * 10];

	lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, TFT_HOR_RES * 10);

	disp_data_regv.hor_res = TFT_HOR_RES;
	disp_data_regv.ver_res = TFT_VER_RES;
	disp_data_regv.flush_cb = tft_flush;
	disp_data_regv.draw_buf = &disp_buf;

	lv_disp_drv_register(&disp_data_regv);
}

/*****************************************************************************/

static void tft_flush(lv_disp_drv_t *data_regv, const lv_area_t *area, lv_color_t *color_p)
{
    /*Return if the area is out the screen*/
    if(area->x2 < 0) return;
    if(area->y2 < 0) return;
    if(area->x1 > TFT_HOR_RES - 1) return;
    if(area->y1 > TFT_VER_RES - 1) return;

    setAddrWindow(area->x1, area->y1, area->x2, area->y2);
    pushColors((uint16_t*)color_p, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);

    lv_disp_flush_ready(data_regv);
}

/*****************************************************************************/

static void setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	writeCommand(ILI9486_COLADDRSET);

	writeData(x1>>8);
	writeData(x1);

	writeData(x2>>8);
	writeData(x2);

	writeCommand(ILI9486_PAGEADDRSET);

	writeData(y1>>8);
	writeData(y1);

	writeData(y2>>8);
	writeData(y2);
}

/*****************************************************************************/

void pushColors(uint16_t *data, int16_t len, bool first)
{
  uint16_t color;
  uint8_t  hi, lo;
  CS_ACTIVE;
  if(first == true)
  { // Issue GRAM write command only on first call
    CD_COMMAND;
    writeByte(0x2C);
  }
  CD_DATA;
  while(len--)
  {
    color = *data++;
    hi    = color >> 8; // Don't simplify or merge these
    lo    = color;      // lines, there's macro shenanigans
    writeByte(hi);         // going on.
    writeByte(lo);
  }
  CS_IDLE;
}

/*****************************************************************************/

void ili9486_init(void)
{
	//init tft pins
	CS_IDLE;
	CD_DATA;
	WR_IDLE;
	RD_IDLE;
	RST_TOGGLE;

	writeCommand(0x01);

	HAL_Delay(150);

	writeCommand(0x28);

	writeCommand(0x3A);
	writeData(0x55);

	//Power Control 1
	writeCommand(0xC0);
	writeData(0x0d);
	writeData(0x0d);

	//Power Control 2
	writeCommand(0xC1);
	writeData(0x43);
	writeData(0x00);

	//Power Control 3
	writeCommand(0xC2);
	writeData(0x00);

	//VCOM  Control 1
	writeCommand(0xC5);
	writeData(0x00);
	writeData(0x48);
	writeData(0x00);
	writeData(0x48);

	//Inversion Control
	writeCommand(0xB4);
	writeData(0x00);

	//Display Function Control
	writeCommand(0xB6);
	writeData(0x02);
	writeData(0x02);
	writeData(0x3B);

	//Positive Gamma Correction
	writeCommand(0xE0);
	writeData(0x0F);
	writeData(0x21);
	writeData(0x1C);
	writeData(0x0B);
	writeData(0x0E);
	writeData(0x08);
	writeData(0x49);
	writeData(0x98);
	writeData(0x38);
	writeData(0x09);
	writeData(0x11);
	writeData(0x03);
	writeData(0x14);
	writeData(0x10);
	writeData(0x00);

	//Negative Gamma Correction
	writeCommand(0xE1);
	writeData(0x0F);
	writeData(0x2F);
	writeData(0x2B);
	writeData(0x0C);
	writeData(0x0E);
	writeData(0x06);
	writeData(0x47);
	writeData(0x76);
	writeData(0x37);
	writeData(0x07);
	writeData(0x11);
	writeData(0x04);
	writeData(0x23);
	writeData(0x1E);
	writeData(0x00);

	//Sleep Out
	writeCommand(0x11);

	HAL_Delay(150);

	//Display On
	writeCommand(0x29);

	//Invert Off
	writeCommand(ILI9486_INVERTOFF);

	//Memory Access
	writeCommand(ILI9486_MADCTL);
	//writeData(0x48); //portrait
	//writeData(0x88); //portrait * 180
	//writeData(0x28); //landscape
	writeData(0xE8); //landscape * 180

	//RGB Signal
	writeCommand(0xB0);
	writeData(0x40);
}

/*****************************************************************************/

void writeCommand(uint8_t c)
{
	CS_ACTIVE;
	CD_COMMAND;
	writeByte(c);
	CS_IDLE;
}

/*****************************************************************************/

void writeData(uint8_t c)
{
	CS_ACTIVE;
	CD_DATA;
	writeByte(c);
	CS_IDLE;
}
