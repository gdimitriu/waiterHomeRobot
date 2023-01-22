/*
 * Stage 2 - LCD operations
 * Copyright 2023 Gabriel Dimitriu
 *
 * This file is part of waiterHomeRobot project.

 * waiterHomeRobot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * waiterHomeRobot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with waiterHomeRobot; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
*/

#include <SPI.h>
#include <Ucglib.h>
#include "lcd_operations.h"

#define BEGIN_CUSTOM_MESSAGES 36
static Ucglib_ILI9341_18x240x320_HWSPI ucg(/*cd=*/ DC_LCD , /*cs=*/ CS_LCD, /*reset=*/ RST_LCD);

static ucg_int_t currentPosition;

void initLCD() {
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.clearScreen();
  ucg.setColor(0, 80, 40, 0);
  ucg.setColor(1, 60, 0, 40);
  ucg.setColor(2, 20, 0, 20);
  ucg.setColor(3, 60, 0, 0);
  ucg.setRotate90();
  currentPosition = BEGIN_CUSTOM_MESSAGES;
}

void resetLinePos() {
  ucg.setColor(0, 0, 0);
  ucg.drawBox(0, BEGIN_CUSTOM_MESSAGES, ucg.getWidth(), ucg.getHeight());
  currentPosition = BEGIN_CUSTOM_MESSAGES;
}

bool printNextLineText(char *text, uint8_t fontSize) {
  switch(fontSize) {
    case 8:
      ucg.setFont(ucg_font_helvB08_hr);
      currentPosition  += 13;
      if (currentPosition > ucg.getHeight()) {
        return false;
      }
      break;
    case 24:
      ucg.setFont(ucg_font_helvB24_hr);
      currentPosition += 29;
      if (currentPosition > ucg.getHeight()) {
        return false;
      }
      break;
  }
  ucg.setColor(255, 255, 255);
  ucg.setPrintPos(2, currentPosition);
  ucg.setPrintDir(0);
  ucg.print(text);
  return true;
}

void printCurrentFilePath(char *filePath) {
  ucg.setFont(ucg_font_helvB12_hr);
  ucg.setColor(0, 255, 0);
  ucg.drawBox(0, 0, ucg.getWidth(), BEGIN_CUSTOM_MESSAGES);
  ucg.setColor(255, 0, 0);
  ucg.setPrintDir(0);
  ucg.setPrintPos(2,17);
  ucg.print("Current file path");
  ucg.setPrintPos(2,BEGIN_CUSTOM_MESSAGES - 2);
  ucg.setPrintDir(0);
  ucg.print(filePath);
}
