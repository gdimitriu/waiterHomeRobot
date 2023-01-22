/*
 * Stage 2
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

#ifdef LCD_TEST
static void printInSetup() {
  printCurrentFilePath("route 1");
  printNextLineText("line 1",24);
  printNextLineText("line 2",24);
  printNextLineText("line 3",24);
  printCurrentFilePath("route 02");
  printNextLineText("line 4",24);  
}
static void printInLoop() {
  delay(5000);
  char buffer[50];
  memset(buffer,0,sizeof(char)*50);
  resetLinePos();
  for(int i = 0; i < 7; i++) {
    memset(buffer,0,sizeof(char)*50);
    sprintf(buffer,"line %d",i);
    printNextLineText(buffer,24);
  }
  for(int i = 0; i < 1000; i++) {
    memset(buffer,0,sizeof(char)*50);
    sprintf(buffer,"route %d",i);
    printCurrentFilePath(buffer);
    delay(1000);    
  }
}
#endif

void setup() {
  initCommunications();
  initLCD();
  initSDCardReader();
#ifdef LCD_TEST
  printInSetup();
#endif
}

void loop() {
  //receiveCommand();
#ifdef LCD_TEST
  printInLoop();
#endif
}
