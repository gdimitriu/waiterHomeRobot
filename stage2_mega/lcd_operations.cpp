/*
 * Stage 2 - LCD operations
 * 
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

#define CUSTOM_MESSAGES 36
static Ucglib_ILI9341_18x240x320_HWSPI ucg(/*cd=*/ DC_LCD , /*cs=*/ CS_LCD, /*reset=*/ RST_LCD);

static ucg_int_t currentPosition;

void initLCD() {
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.clearScreen();
  ucg.setColor(0, 80, 40, 0);
  ucg.setColor(1, 60, 0, 40);
  ucg.setColor(2, 20, 0, 20);
  ucg.setColor(3, 60, 0, 0);
  ucg.setRotate270();
  currentPosition = CUSTOM_MESSAGES;
  ucg.setFont(ucg_font_helvB12_hr);
  ucg.setColor(0, 255, 0);
  ucg.drawBox(0, 0, ucg.getWidth(), CUSTOM_MESSAGES);
  ucg.setColor(255, 0, 0);
  ucg.setPrintDir(0);
  ucg.setPrintPos(2,17);
  ucg.print("Current file path");
}

void resetLinePos() {
  ucg.setColor(0, 0, 0);
  ucg.drawBox(0, CUSTOM_MESSAGES, ucg.getWidth(), ucg.getHeight() - 2 * CUSTOM_MESSAGES );
  currentPosition = CUSTOM_MESSAGES;
}

bool printNextLineText(char *text, uint8_t fontSize) {
  switch(fontSize) {
    case 8:
      ucg.setFont(ucg_font_helvB08_hr);
      currentPosition  += 13;
      if ( currentPosition > ( ucg.getHeight() - CUSTOM_MESSAGES ) ) {
        return false;
      }
      break;
    case 24:
      ucg.setFont(ucg_font_helvB24_hr);
      currentPosition += 29;
      if ( currentPosition > ( ucg.getHeight() - CUSTOM_MESSAGES ) ) {
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
  ucg.drawBox(0, 0, ucg.getWidth(), CUSTOM_MESSAGES);
  ucg.setColor(255, 0, 0);
  ucg.setPrintDir(0);
  ucg.setPrintPos(2,17);
  ucg.print("Current file path");
  ucg.setPrintPos(2,CUSTOM_MESSAGES - 2);
  ucg.setPrintDir(0);
  ucg.print(filePath);
}


void printCurrentACCPower(float value) {
  ucg.setFont(ucg_font_helvB24_hr);
  ucg.setColor(0, 255, 0);
  ucg.drawBox(0, ucg.getHeight() - CUSTOM_MESSAGES, ucg.getWidth(), ucg.getHeight());
  ucg.setColor(255, 0, 0);
  ucg.setPrintDir(0);
  ucg.setPrintPos(2,ucg.getHeight() - 7);
  char buffer[50];
  char szF[7];
  memset(szF,0,sizeof(char)*7);
  dtostrf( value, 4, 2, szF );
  memset(buffer,0,sizeof(char)*50);
  sprintf(buffer,"Power : %s V",szF);
  ucg.print(buffer);
}

/**********************************************************************
 *
 * standalone tests enabled by TEST_LCD flag
 *
 **********************************************************************/
#ifdef TEST_LCD

  void printInSetupLcdTest() {
    printCurrentFilePath("route 1");
    printNextLineText("line 1",24);
    printNextLineText("line 2",24);
    printNextLineText("line 3",24);
    printNextLineText("line 4",24);  
  }

  void printInLoopLcdTest() {
    delay(5000);
    char buffer[50];
    memset(buffer,0,sizeof(char)*50);
    resetLinePos();
    for(int i = 0; i < 7; i++) {
      memset(buffer,0,sizeof(char)*50);
      sprintf(buffer,"line %d",i);
      printNextLineText(buffer,24);
    }
    for(int i = 0; i < 10; i++) {
      memset(buffer,0,sizeof(char)*50);
      sprintf(buffer,"route %d",i);
      printCurrentFilePath(buffer);
      delay(1000);    
    }
  }
#endif
