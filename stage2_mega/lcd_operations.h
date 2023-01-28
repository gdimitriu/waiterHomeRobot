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

#ifndef __LCD_OPERATIONS_H__
#define __LCD_OPERATIONS_H__
#include "configuration.h"
#include <Arduino.h>

/**********************************************
 * 
 * initialize the LCD
 * 
 **********************************************/
extern void initLCD();

/**********************************************
 * 
 * print the text with fontsize
 * return true if had space on LCD otherwise false
 * 
 **********************************************/
extern bool printNextLineText(char *text,uint8_t fontSize);

/**********************************************
 * 
 * reset the line position
 * 
 **********************************************/
extern void resetLinePos();

/**********************************************
 * 
 * print the current file path
 * the file path contains the commands to the
 * moving engines processor
 * 
 **********************************************/
extern void printCurrentFilePath(char *filePath);

/**********************************************************************
 *
 * standalone tests enabled by TEST_LCD flag
 *
 **********************************************************************/
#ifdef TEST_LCD
  extern void printInSetupLcdTest();
  extern void printInLoopLcdTest();
#endif

#endif
