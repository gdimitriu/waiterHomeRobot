/*
 * Stage 2 - communication module
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
#include <Arduino.h>

#define SIZE_BUFFER_BRAIN 256
static char inDataBrain[SIZE_BUFFER_BRAIN];

#define SIZE_BUFFER_ENGINES 50 // same as in moving engines
static char inDataEngines[SIZE_BUFFER_ENGINES];

static int enginesIndex;

static int brainIndex;

void initCommunications() {
  //initialize serial to moving engine processor
  Serial2.begin(38400);
  //initialize serial to brain or BLE if had no brain
  Serial3.begin(38400);
}

static void makeCleanupBrain() {
  for ( brainIndex = 0; brainIndex < SIZE_BUFFER_BRAIN; brainIndex++ ) {
    inDataBrain[brainIndex] = '\0';
  }
  brainIndex = 0;
  Serial3.flush();
}

static void makeCleanupEngines() {
  for ( enginesIndex = 0; enginesIndex < SIZE_BUFFER_ENGINES; enginesIndex++ ) {
    inDataEngines[enginesIndex] = '\0';
  }
  enginesIndex = 0;
  Serial2.flush();
}

static void sendCommandToEngines() {
  makeCleanupEngines();
  Serial2.print(inDataBrain);
  Serial2.flush();    
  makeCleanupBrain();
  isForMeFromEngines = false;
}

static boolean isEnginesCommand() {
  //single command + #\r\n
  if ( strlen(inDataBrain) == 4 ) {
    return true;
  }
  //special command with data
  switch (inDataBrain[0]) {
    case 'v': //set minPower
    case 'V': //set maxPower
    case 'm': //move/rotate with distance
    case 'M': //move/rotate by human
    case 'c': //set current power
    case 'r': //RFID operations
      return true;
    default:
      return false;
  }
}

static void processLcdCommand() {
  
}

static void processCommand() {
  if ( isEnginesCommand ) { 
    //single command are routed to the engines processor and wait reply and send back data to brain
    sendCommandToEngines();
  } else { //it is for me
    switch (inDataBrain[0]) {
      case 'l': //lcd operation
        processLcdCommand();
        break;
      default:
        cleanupBrain();
        sprintf(inDataBrain,"unknown operation\r\n");
        Serial3.print(inDataBrain);
        Serial3.flush();
        cleanupBrain();
    }
  }
}

void receiveCommand() {
  boolean isBrain = false;
  boolean isEngines = false;
  while ( Serial2.available() > 0 || Serial3.available() > 0 ) {
    //order comes from BLE or brain
    if ( Serial3.available() > 0 ) {
      if ( brainIndex < ( SIZE_BUFFER_BRAIN - 1 ) ) {
        char inChar = Serial3.read(); // Read a character
        boolean isData = true;
        if ( inChar=='\r' || inChar=='\n' || inChar =='\0' || inChar < 35 || inChar > 122 ) {
          isData = false;
        } else 
        //commands start with a letter capital or small
        if ( brainIndex == 0 && !( ( inChar >64 && inChar <91 ) || ( inChar > 96 && inChar<123 ) ) ) {
          isData = false;
        }
        if ( isData ) {
          inDataBrain[brainIndex++] = inChar; // Store it
          inDataBrain[brainIndex] = '\0'; // Null terminate the string
          if ( inChar == '#' ) {
            isBrain = true;
            break;
          }
        }
      } else { //too big something is wrong
        makeCleanupBrain();
      }
    }
    //receive comes from moving processor as a reply for me
    if ( Serial2.available() > 0 ) {
      if ( enginesIndex < ( SIZE_BUFFER_ENGINES - 1 ) ) {
        char inChar = Serial2.read(); // Read a character
        boolean isData = true;
        inDataEngines[enginesIndex++] = inChar; // Store it
        inDataEngines[enginesIndex] = '\0'; // Null terminate the string
        if ( inChar == '\n' ) {
            isEngines = true;
            break;
        }
      } else { //too big something is wrong
        makeCleanupEngines();
      }
    }
  }

  if ( isBrain ) {
    if ( brainIndex >= 1 ) {
      if ( inDataBrain[brainIndex - 1] == '#' ) {
        processCommand();
        isBrain = false;
      }
    }
  }
  
  if ( isEngines ) {
    if ( enginesIndex >= 2 ) {
      if ( inDataEngines[enginesIndex - 1] == '\n' && inDataEngines[enginesIndex - 2] == '\r' ) {
         Serial3.print(inDataEngines);
         Serial3.flush();
         makeCleanupEngines();
      }
    }
  }
}
