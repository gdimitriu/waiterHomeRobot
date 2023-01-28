/*
 * Stage 2 - SD card operations
 * 
 * This is used only for LCD and other operations not for Sound Module.
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

#include <SD.h>
#include "sd_operations.h"
#include "lcd_operations.h"
#define LINE_SIZE 256

static bool alreadyBegan = false;  // SD.begin() misbehaves if not first call
static char lineBuffer[LINE_SIZE];
static File fd;

void initSDCardReader() {
  pinMode(CARD_DETECT_SD, INPUT);
  // Is there even a card?
  if (!digitalRead(CARD_DETECT_SD))
  {
    while (!digitalRead(CARD_DETECT_SD));
    delay(250); // 'Debounce insertion'
  }
  
  // Card seems to exist.  begin() returns failure
  // even if it worked if it's not the first call.
  if (!SD.begin(CHIP_SELECT_SD) && !alreadyBegan)  // begin uses half-speed...
  {
    initSDCardReader(); // Possible infinite retry loop is as valid as anything
  }
  else
  {
    alreadyBegan = true;
  }
}

bool openFile(char *fileName) {
  if (fd) {
    fd.close();
  }
  fd = SD.open(fileName);
  if (!fd || !SD.exists(fileName)) {
    return false;
  }
  return true;
}

void closeFile() {
  if(fd) {
    fd.close();
  }
}

char* readNextLine() {
  memset(lineBuffer, 0, sizeof(char)*LINE_SIZE);
  int index = 0;
  while (fd.available()) {
    char ch = fd.read();      
    if (ch == '\n') {
      return lineBuffer;
    } else {
      if (index > LINE_SIZE) {
        return lineBuffer;
      }
      lineBuffer[index++] = ch;
      lineBuffer[index] = '\0';
    }
  }
  return NULL;
}

void readFullFile(char *buffer, unsigned int size) {
  memset(buffer, 0, sizeof(char)*size);
  int index = 0;
  while (fd.available()) {
    char ch = fd.read();      
    if (index > size) {
      return;
    }
    buffer[index++] = ch;
    buffer[index] = '\0';
  }
}

#ifdef TEST_SD_LCD  
  void printInSetupSdLcdTest() {
    Serial.begin(38400);
    char fileName[] = "tests/sd_lcd.txt";
    printCurrentFilePath(fileName);
    if(openFile("tests/sd_lcd.txt")) {
      char * line = readNextLine();    
      while(line != NULL) {
        printNextLineText(line,24);
        Serial.println(line);
        line = readNextLine();
      }
      closeFile();
    } else {
      Serial.println("File does not exist");
    }
  }
#endif
