/*
 * Moving engines processor
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

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PWMServoDriver.h>
#include <NeoSWSerial.h>
#include "configuration.h"
#include "MoveEngines.h"
#include "rfid.h"
#include "ColissionSensors.h"

Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver(0x40);

int maxPower = ABSOLUTE_MAX_POWER;
int currentPower = maxPower;
int minPower = 2000;
// for input data from ble
boolean cleanupSerial;
bool isValidInput;
char inData[SERIAL_BUFFER]; // Allocate some space for the string
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character

NeoSWSerial BTSerial(RxD, TxD);

void setup()
{
    //enable serial
    BTSerial.begin(38400);     
    //init the PWM driver
    pwmDriver.begin(PCA9685_ADDRESS);
    pwmDriver.setOscillatorFrequency(27000000);
    pwmDriver.setPWMFreq(50.0);
    Wire.setClock(400000);
    SPI.begin(); // Init SPI bus
    isValidInput = false;
    cleanupSerial = false;
    engineSetup();
#ifdef HAS_RFID    
    initRFID();
#endif
#ifdef HAS_COLISSIONS_SENSORS    
    initColissionSensors();
#endif
}

boolean isValidNumber(char *data, int size)
{
  if (size == 0 ) return false;
   boolean isNum=false;
   if(!(data[0] == '+' || data[0] == '-' || isDigit(data[0]))) return false;

   for(byte i=1;i<size;i++)
   {
       if(!(isDigit(data[i]) || data[i] == '.')) return false;
   }
   return true;
}

boolean makeCleanup() {
  for (index = 0; index < SERIAL_BUFFER; index++) {
    inData[index] = '\0';
  }
  index = 0;
  inChar ='0';
  BTSerial.flush();
}

void sendOK() {
  BTSerial.println("OK");
  BTSerial.flush();
}

bool setMaxPowerCommand() {
  sendOK();
  //remove V from command
  for (uint8_t i = 0 ; i < strlen(inData); i++) {
    inData[i]=inData[i+1];
  }
  if (!isValidNumber(inData, index - 2)) {
    isValidInput = false;
    makeCleanup();
    return false;
  }
  if (atol(inData) > ABSOLUTE_MAX_POWER || atol(inData) < 0) {
    isValidInput = false;
    makeCleanup();
    return false;
  }
  maxPower = atol(inData);
  makeCleanup();
  isValidInput = true;
  return true;
}

bool setMinPowerCommand() {
  sendOK();
  //remove v from command
  for (uint8_t i = 0 ; i < strlen(inData); i++) {
    inData[i]=inData[i+1];
  }
  if (!isValidNumber(inData, index - 2)) {
    isValidInput = false;
    makeCleanup();
    return false;
  }
  if (atol(inData) > maxPower || atol(inData) < 0) {
    isValidInput = false;
    makeCleanup();
    return false;
  }
  minPower = atol(inData);
  makeCleanup();
  isValidInput = true;
  return true;
}

bool setCurrentPowerCommand() {
  sendOK();
  //remove c from command
  for (uint8_t i = 0 ; i < strlen(inData); i++) {
    inData[i]=inData[i+1];
  }
  if (!isValidNumber(inData, index - 2)) {
    isValidInput = false;
    makeCleanup();
    return false;
  }
  if (atol(inData) > maxPower || atol(inData) < 0) {
    isValidInput = false;
    makeCleanup();
    return false;
  }
  currentPower = atol(inData);
  makeCleanup();
  isValidInput = true;
  return true;
}

bool processPlatformUnsupportedCommand() {
  sendOK();
  makeCleanup();
  isValidInput = true;
  return true;
}

bool moveOrRotateUntilNextCommand() {
  resetInterrupt();
  disableEncoders();
  //remove M from command
  for (uint8_t i = 0 ; i < strlen(inData); i++) {
    inData[i]=inData[i+1];
  }
  inData[strlen(inData)] = '\0';
  int position;
  for (uint8_t i = 0; i < strlen(inData); i++) {
    if (inData[i] == ',') {
      position = i;
      break;
    }
  }
  char buf[10];
  for (int i = 0; i < 10; i++) {
    buf[i] = '\0';
  }
  for (int i = 0 ; i < position; i++) {
    buf[i] = inData[i];
  }
  int moveData = atoi(buf);
  for (int i = 0; i < 10; i++) {
    buf[i] = '\0';
  }
  int idx = 0;
  for (int i = position + 1; i < strlen(inData); i++) {
    buf[idx] = inData[i];
    idx++;
  }
  int rotateData = atoi(buf);
  if (moveData == 0 && rotateData == 0) {
    go(0,0);
  } else if (rotateData == 0) {
    if (moveData < 0) {
      go(-currentPower,-currentPower);
    } else {
      go(currentPower, currentPower);
    }
  } else {
    if (rotateData < 0) {
      go(-currentPower,currentPower);
    } else {
      go(currentPower, -currentPower);
    }
  }
  makeCleanup();
  isValidInput = true;
  return true;
}

bool moveOrRoatateWithDistanceCommand() {
  enableEncoders();
  //remove m from command
  for (uint8_t i = 0 ; i < strlen(inData); i++) {
    inData[i]=inData[i+1];
  }
  inData[strlen(inData)] = '\0';
  int position;
  for (uint8_t i = 0; i < strlen(inData); i++) {
    if (inData[i] == ',') {
      position = i;
      break;
    }
  }
  char buf[10];
  for (int i = 0; i < 10; i++) {
    buf[i] = '\0';
  }
  for (int i = 0 ; i < position; i++) {
    buf[i] = inData[i];
  }
  int moveData = atoi(buf);
  for (int i = 0; i < 10; i++) {
    buf[i] = '\0';
  }
  int idx = 0;
  for (int i = position + 1; i < strlen(inData); i++) {
    buf[idx] = inData[i];
    idx++;
  }
  int rotateData = atoi(buf);
  if (moveData == 0 && rotateData == 0) {
    go(0,0);
  } else if (rotateData == 0) {
    moveLinear(moveData);
  } else {
    rotateDegree(rotateData);
  }
  makeCleanup();
  isValidInput = true;
  return true;
}

bool makeMove() {
  char buffer[SERIAL_BUFFER];
  for (int i = 0; i < SERIAL_BUFFER; i++) {
    buffer[i] = '\0';
  }
  if (index > 0) {
     inData[index-1] = '\0';
  }
  if (strlen(inData) == 1) {
    if (inData[0] == 'I') {
      sprintf(buffer,"unsupported\r\n");
      BTSerial.print(buffer);
      BTSerial.flush();
    } else if (inData[0] == 'V') {      
      sprintf(buffer,"%d\r\n",maxPower);
      BTSerial.print(buffer);
      BTSerial.flush();
    } else if (inData[0] =='v') {
      sprintf(buffer,"%d\r\n",minPower);
      BTSerial.print(buffer);
      BTSerial.flush();
    } else if (inData[0] =='c') {
      sprintf(buffer,"%d\r\n",currentPower);
      BTSerial.print(buffer);
      BTSerial.flush();
    }  else if (inData[0] == 'd') {
      sprintf(buffer,"%d\r\n",0);
      BTSerial.print(buffer);
      BTSerial.flush();
    } else if (inData[0] == 's') {
      sprintf(buffer,"%d\r\n",0);
      BTSerial.print(buffer);
      BTSerial.flush();
    } else if (inData[0] == 'b') { 
      breakAllEngines();
    } else if (inData[0] == 'C') {
      sprintf(buffer,"%d:%d:%d:%d\r\n",getLeftFrontEncoderCount(),getRightFrontEncoderCount(),getLeftBackEncoderCount(),getRightBackEncoderCount());
      BTSerial.print(buffer);
      BTSerial.flush();
    } else if (inData[0] == 'R') {
      enableEncoders();
#ifdef SERIAL_DEBUG_MODE      
      sprintf(buffer,"%d:%d:%d:%d\r\n",getLeftFrontEncoderCount(),getRightFrontEncoderCount(),getLeftBackEncoderCount(),getRightBackEncoderCount());
#else
      sprintf(buffer,"%d\r\n",0);
#endif      
      resetCounters();      
      BTSerial.print(buffer);
      BTSerial.flush();
    } else {
      sprintf(buffer,"%d\r\n",0);
      BTSerial.print(buffer);
      BTSerial.flush();
      makeCleanup();
      isValidInput = false;
      return false;
    }
  } else {
      if (inData[0] == 'V') {
        return setMaxPowerCommand();
      } else if (inData[0] == 'v') {
        return setMinPowerCommand();
      } else if (inData[0] == 'c') {
        return  setCurrentPowerCommand();
      } else if (inData[0] == 'd') {
        //this does not support low power distance
        return processPlatformUnsupportedCommand();
      } else if (inData[0] == 's') {
        //this does not support stop distance
        return processPlatformUnsupportedCommand();
      } else if (inData[0] == 'M') {
        return moveOrRotateUntilNextCommand();
      } else if (inData[0] == 'm') {
        bool moving = moveOrRoatateWithDistanceCommand();
        if (isInterrupted()) {
          float *distances = getCurrentDistances();
          sprintf(buffer,"I%f,%f,%f,%f#", distances[0], distances[1], distances[2], distances[3]);
          BTSerial.print(buffer);
          BTSerial.flush();
           resetInterrupt();
        }       
      } else {
        sprintf(buffer,"%d\r\n",0);
        BTSerial.print(buffer);
        BTSerial.flush();
        makeCleanup();
        isValidInput = false;
        return false;
      }
    }
    isValidInput = true;
    makeCleanup();
    return true;
}

void loop()
{
#ifdef HAS_RFID
  if (isCardPresent()) {
    breakAllEngines();
    uint8_t readed;
    readRFID(inData, readed);
    BTSerial.write(inData);
    makeMove();
    return;
  }
#endif
  while(BTSerial.available() > 0) // Don't read unless there you know there is data
  {
     if(index < 19) // One less than the size of the array
     {
        inChar = BTSerial.read(); // Read a character
        if (inChar=='\r' || inChar=='\n' || inChar =='\0' || inChar < 35 || inChar > 122) {
          continue;
        }
        //commands start with a letter capital or small
        if (index == 0 && !((inChar >64 && inChar <91) || (inChar > 96 && inChar<123))) {
          continue;
        }    
        inData[index++] = inChar; // Store it
        inData[index] = '\0'; // Null terminate the string
        if (inChar == '#') {
          break;
        }
     } else {
        makeCleanup();
        cleanupSerial = true;
     }
 }
 if (index >= 1) {
  if (inData[index - 1] == '#') {
    makeMove();
  } else if (cleanupSerial) {
    makeCleanup();
    cleanupSerial = false;
  } else {
    delay(10);
  }
 }
}
