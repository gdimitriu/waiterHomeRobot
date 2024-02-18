/*
 * Stage 2 main entry point.
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

#include "communication.h"
#include "lcd_operations.h"
#include "sd_operations.h"
#include "sound_module.h"
#include "rfid.h"

void setup() {
  //for debug reasons
  //Serial.begin(38400);
#ifdef TEST_SD_LCD
  Serial.begin(38400);
#endif
  initCommunications();
  initLCD();
#ifdef TEST_LCD
  printInSetupLcdTest();
#endif
#ifdef HAS_SD
  initSDCardReader();
#endif
#ifdef HAS_RFID
  initRFID();
#endif
  initSoundModule();

#ifdef HAS_SD
#ifdef TEST_SD_LCD  
  printInSetupSdLcdTest();
#endif
#endif
}

void loop() {
  receiveCommand();
#ifdef TEST_LCD
  printInLoopLcdTest();
#endif
}
