/*
 * Moving engines processor - RFID reader
 * Copyright 2022 Gabriel Dimitriu
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
#ifndef __RFID_H__
#define __RFID_H__
#include <Arduino.h>
/**********************************************
 * 
 * size of the mifare ultralight C 36 pages
 * 
 **********************************************/
#define ULTRALIGHT_C_PAGES 31
#define BUFFER_SIZE (ULTRALIGHT_C_PAGES*4)

/**********************************************
 * 
 * init the rfid
 * 
 **********************************************/
void initRFID();

/**********************************************
 * 
 * return true if a new rfid sticker is present
 * 
 **********************************************/
bool isCardPresent();

/**********************************************
 * 
 * read the rfid sticker
 * 
 **********************************************/
bool readRFID(char *inData, uint8_t &size);

#endif
