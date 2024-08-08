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
#include <Arduino.h>
#include "communication.h"
#include "power_monitoring.h"
#include "sd_operations.h"
#include "lcd_operations.h"
#include "ultrasonics.h"
#include "path_navigation.h"


#define SIZE_BUFFER_BRAIN 256
static char inDataBrain[SIZE_BUFFER_BRAIN];
static char outDataBrain[SIZE_BUFFER_BRAIN];

static int brainIndex;

void initCommunications() {
  //initialize serial to moving engine processor
  ENGINES_PROC_SERIAL.begin(38400);
  //initialize serial to brain or BLE if has no brain
#ifndef IS_STANDALONE  
  BRAIN_COMM_SERIAL.begin(38400);
#endif  
}

static void makeCleanupBrain() {
  for ( brainIndex = 0; brainIndex < SIZE_BUFFER_BRAIN; brainIndex++ ) {
    inDataBrain[brainIndex] = '\0';
  }
  brainIndex = 0;
  BRAIN_COMM_SERIAL.flush();
}

static void sendCommandToEngines(char *inData) {
  ENGINES_PROC_SERIAL.print(inData);
  ENGINES_PROC_SERIAL.flush();    
  makeCleanupBrain();
}

static void processAutoCommand(char *dataIn) {
  char *inData = dataIn;
  inData++;
  inData[strlen(inData)] = '\0';
  int position;
  for ( uint8_t i = 0; i < strlen(inData); i++ ) {
    if ( inData[i] == ',' ) {
      position = i;
      break;
    }
  }
  char buf[10];
  memset(buf,0,10*sizeof(char));
  for ( int i = 0 ; i < position; i++ ) {
    buf[i] = inData[i];
  }
  int moveData = atoi(buf);
  memset(buf,0,10*sizeof(char));
  int idx = 0;
  for ( int i = position + 1; i < strlen(inData); i++ ) {
    buf[idx] = inData[i];
    idx++;
  }
  int rotateData = atoi(buf);
  if ( moveData == 0 && rotateData == 0 ) {
    
  } else if ( rotateData == 0 ) {
    if ( moveData > 0 ) {
      long distance = getFrontDistance(90);
      if ( distance > lowPowerDistance ) {
        
      } else if ( distance < stopDistance ) {
        
      }
    } else {
      long distance = getRearDistance(90);
      if ( distance > lowPowerDistance ) {
        
      } else if ( distance < stopDistance ) {
        
      }
    }
  } else {
    
  }
  //send back command to engines
  sendCommandToEngines(dataIn);
}

static boolean isEnginesCommand(char *dataIn) {
  //special command with data
  switch (dataIn[0]) {
    case 'v': //set minPower
    case 'V': //set maxPower
    case 'm': //move/rotate with distance
    case 'M': //move/rotate by human
    case 'c': //set current power
    case 'b': //break engines
    case 'C': //get encoder values
    case 'R': //reset encoders
    case 'r': //RFID operations
      return true;
    default:
      return false;
  }
}

static boolean isAutoCommand(char *dataIn) {
  switch (dataIn[0]) {
    case 'm': //move/rotate with distance
      return true;
    default:
      return false;
  }
}

static void printLcdLine(char *realData) {
  char buff[5];
  memset(buff,'\0',5*sizeof(char));
  char *pBuff = buff;
  while ( *realData != ',' ) {
    *pBuff = *realData;
    pBuff++;
    realData++;
  }
  //remove ,
  realData++;
  printNextLineText(realData,atoi(buff));  
}

static void printFileOnLcd(char *realData) {
  char buff[5];
  memset(buff,'\0',5*sizeof(char));
  char *pBuff = buff;
  while ( *realData != ',' ) {
    *pBuff = *realData;
    pBuff++;
    realData++;
  }
  //remove ,
  realData++;
  if (!openFile(realData)) {
    return;
  }
  resetLinePos();
  char *lineRead = readNextLine();
  int fontSize = atoi(buff);
  while (  printNextLineText(lineRead, fontSize) ) {
    lineRead = readNextLine();
    if ( lineRead == NULL)
      break;
  }
  closeFile();
}

static void processLcdCommand() {
  char *realData = inDataBrain;
  //remove l
  realData++;
  //remove #
  realData[strlen(realData) - 1] = '\0';
#ifdef SERIAL_DEBUG
  Serial.println(realData);
  Serial.flush();
#endif  
  if ( strlen(realData) == 1 ) {
    //single commands
    switch ( *realData ) {
      //clear lcd
      case 'c' :
        resetLinePos();
        break;
      case 's' :
        resetLinePos();
        showStartMessage();
        break;
    }
  } else {
    switch ( *realData ) {
      case 'n' :
        //remove n
        realData++;
        //remove ,
        realData++;
        printLcdLine(realData);
        break;
      case 'f' :
        //remove f
        realData++;
        //remove ,
        realData++;
        printFileOnLcd(realData);
        break;
    }
  }
  makeCleanupBrain();
}

static void processAccPowerLevelCommand() {
  BRAIN_COMM_SERIAL.print(getPowerLevel());
  BRAIN_COMM_SERIAL.flush();
  makeCleanupBrain();
}

static void processSoundCommand() {
  //remove P
  char *temp = inDataBrain + 1;
  //remove #
  temp[strlen(temp)] = '\0';
  playSound(temp);
  makeCleanupBrain();
}

static boolean isValidNumber( char *data, int size )
{
  if ( size == 0 ) return false;
   if( !( data[0] == '+' || data[0] == '-' || isDigit(data[0]) ) ) return false;

   for( byte i=1; i<size; i++ )
   {
       if( !( isDigit(data[i]) || data[i] == '.' ) ) return false;
   }
   return true;
}

static void sendBackStopDistance() {
  memset(outDataBrain,0,SIZE_BUFFER_BRAIN * sizeof(char));
  sprintf(outDataBrain,"%ld\n\r",stopDistance);
  BRAIN_COMM_SERIAL.print(outDataBrain);
  BRAIN_COMM_SERIAL.flush();
  makeCleanupBrain();
}

static void setStopDistance() {
  BRAIN_COMM_SERIAL.println("OK");
  BRAIN_COMM_SERIAL.flush();
  //remove s from command
  for ( uint8_t i = 0 ; i < strlen(inDataBrain); i++ ) {
    inDataBrain[i]=inDataBrain[i+1];
  }
  if ( !isValidNumber(inDataBrain, brainIndex - 2) ) {
    makeCleanupBrain();
    return;
  }
  if ( atol(inDataBrain) < 0 ) {
    makeCleanupBrain();
    return;
  }
  stopDistance = atol(inDataBrain);
  makeCleanupBrain();
  return;
}

static void sendBackLowPowerDistance() {
  memset(outDataBrain,0,SIZE_BUFFER_BRAIN * sizeof(char));
  sprintf(outDataBrain,"%ld\n\r",lowPowerDistance);
  BRAIN_COMM_SERIAL.print(outDataBrain);
  BRAIN_COMM_SERIAL.flush();
  makeCleanupBrain();
}

static void setLowPowerDistance() {
  BRAIN_COMM_SERIAL.println("OK");
  BRAIN_COMM_SERIAL.flush();
  //remove d from command
  for ( uint8_t i = 0 ; i < strlen(inDataBrain); i++ ) {
    inDataBrain[i]=inDataBrain[i+1];
  }
  if ( !isValidNumber(inDataBrain, brainIndex - 2) ) {
    makeCleanupBrain();
    return;
  }
  if ( atol(inDataBrain) < 0 ) {
    makeCleanupBrain();
    return;
  }
  lowPowerDistance = atol(inDataBrain);
  makeCleanupBrain();
  return;

}

static void processPathCommand(char *inData) {
  char *currentData = inData;
  if ( currentData[0] == 'N' ) {
    currentData++;
    if ( currentData[0] == 'f' ) { //file operations
      currentData++;
      if ( currentData[0] == 'l' ) { //load file
        currentData++;
        loadFileInMemory(currentData);
      } else if ( currentData[0] == 's' ) { //save file
        currentData++;
        saveFileFromMemory(currentData);
      } else if (currentData[0] == 'r' ) { //remove file
        removeFile(currentData);
      }
    } else {
      switch ( currentData[0] ) {
        case '\0' : //clear list
          clearList();
          break;
        case 'D' : // move direct
          break;
        case 'B' : //move reverse
          break;
        case 'R' : //move using RFID
          break;
        default: //add command
          currentData[strlen(currentData)] = '#';
          currentData[strlen(currentData)] = '\0';
          addCommand(currentData);
      }
    }
  } else if ( currentData[0] == 'D' ) { //move direct with memory
    setPathDirection(true);
    char* command = getNextCommand();
    while ( command != NULL ) {
      if (command[0] == 'm') {
        processAutoCommand(command);        
      } else if ( isEnginesCommand(command) ) {
        sendCommandToEngines(command);
      }
      command = getNextCommand();
    }
  } else if ( currentData[0] == 'B' ) { //move backwards with memory
    setPathDirection(false);
    char* command = getPreviousCommand();
    while ( command != NULL ) {
      if (command[0] == 'm') {
        processAutoCommand(command);
      } else if ( isEnginesCommand(command) ) {
        sendCommandToEngines(command);
      }
      command = getPreviousCommand();
    }
  }
  makeCleanupBrain();
}

static void processCommand() {
  if ( isEnginesCommand(inDataBrain) ) { 
    //single command are routed to the engines processor and wait reply and send back data to brain
    sendCommandToEngines(inDataBrain);
  } else if ( isAutoCommand(inDataBrain)) {
    processAutoCommand(inDataBrain);
  } else { //it is for me
    switch (inDataBrain[0]) {
      case 'P': //get accumulator power level
        if ( strlen(inDataBrain) == 1 )
          processAccPowerLevelCommand();
        else
          processSoundCommand();
        break;
      case 'l': //lcd operation
        processLcdCommand();
        break;
      case 's':
        if ( strlen(inDataBrain) == 1 )
          sendBackStopDistance();
        else
          setStopDistance();
        break;
      case 'd':
        if ( strlen(inDataBrain) == 1 )
          sendBackLowPowerDistance();
        else
          setLowPowerDistance();
        break;
      case 'N':
      case 'D':
      case 'B':
        processPathCommand(inDataBrain);
        break;
      default:
        makeCleanupBrain();
        sprintf(inDataBrain,"unknown operation\r\n");
        BRAIN_COMM_SERIAL.print(inDataBrain);
        BRAIN_COMM_SERIAL.flush();
        makeCleanupBrain();
    }
  }
}

void receiveCommand() {
  boolean isBrain = false;
  boolean isEngines = false;
  while ( ENGINES_PROC_SERIAL.available() > 0 || BRAIN_COMM_SERIAL.available() > 0 ) {
    //order comes from BLE or brain
    if ( BRAIN_COMM_SERIAL.available() > 0 ) {
      if ( brainIndex < ( SIZE_BUFFER_BRAIN - 1 ) ) {
        char inChar = BRAIN_COMM_SERIAL.read(); // Read a character
        boolean isData = true;
        if ( inChar=='\r' || inChar=='\n' || inChar =='\0' ) {
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
    if ( ENGINES_PROC_SERIAL.available() > 0 ) {
      BRAIN_COMM_SERIAL.write(ENGINES_PROC_SERIAL.read()); // Read a character
      BRAIN_COMM_SERIAL.flush();
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
}
