/*
 * utils to write/read ultralight
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
/* mifare ultralight example (25-02-2018)
 *
 *   RFID-RC522 (SPI connexion)
 *   
 *   CARD RC522      Arduino (UNO)
 *     SDA  -----------  10 (Configurable, see SS_PIN constant)
 *     SCK  -----------  13
 *     MOSI -----------  11
 *     MISO -----------  12
 *     IRQ  ----------- 
 *     GND  -----------  GND
 *     RST  -----------  9 (Configurable, see RST_PIN constant)
 *     3.3V ----------- 3.3V
 *     
 */

#include <SPI.h>
#include <MFRC522.h>
#include "StringList.h"

#define SS_PIN          10
#define RST_PIN         MFRC522::UNUSED_PIN


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::StatusCode status; //variable to get card status
#define ULTRALIGHT_C_PAGES 31
#define BUFFER_SIZE (ULTRALIGHT_C_PAGES*4)

byte buffer[BUFFER_SIZE];  //data transfer buffer (38+2 bytes data+CRC)
byte readBuffer[18]; //16 + 2 CRC
byte size = sizeof(buffer);

uint8_t pageAddr = 0x06;  //We will write/read max 124 bytes.
                          //Ultraligth C mem = 36 pages. 4 bytes per page. 
                          //Pages 0 to 4 are for special functions.           
//for communication
char inDataSerial[20]; // Allocate some space for the string
char inCharSerial; // Where to store the character read
byte indexSerial = 0; // Index into array; where to store the character
boolean cleanupSerial;

StringList listOfCommands;
void printMenu() {
  Serial.println("Ultralight C (36pages) reader and writer");
  Serial.println("****************************************");
  Serial.println("R# read the card");
  Serial.println("c# clear the commands list");
  Serial.println("l# print the commands list");
  Serial.println("W# write the commands list");
  Serial.println("xxx# add the xxx command to the end of the list");
}

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card 
  Serial.println("Starting!");
  if (!mfrc522.PCD_PerformSelfTest())
    Serial.println("Self test failed");
  mfrc522.PCD_Init(); // Init MFRC522 card 
  Serial.println("Started!");
  printMenu();
}


int readRFID() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return -1;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return -1;
  memset(buffer, 0, BUFFER_SIZE);
  
  // Read data ***************************************************
  
  Serial.println("Reading data ... ");
  Serial.flush();
  unsigned int bufferPos = 0;
  for (int j = 0; j < floor(ULTRALIGHT_C_PAGES/4.0); ++j) {
    memset(readBuffer, 0, 18);
    //data in 4 blocks is readed at once.
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(pageAddr+(j*4), readBuffer, &size);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("MIFARE_Read() failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      return 0;
    }
    memcpy(&buffer[bufferPos],readBuffer,16);
    bufferPos += 16;
  }
  Serial.print("Readed data: <<");
  //Dump a byte array to Serial
  for (byte i = 0; i < BUFFER_SIZE; ++i) {
    Serial.print((char)buffer[i]);
  }
  Serial.println(">>");

  mfrc522.PICC_HaltA();
  return 1;
}

int writeRFID() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return -1;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return -1;
  
  // Write data ***********************************************
  
  for (int i=0; i < BUFFER_SIZE/4; i++) {
    //data is writen in blocks of 4 bytes (4 bytes per page)
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Ultralight_Write(pageAddr+i, &buffer[i*4], 4);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Ultralight_Write() failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      return 0;
    }
  }
  Serial.println("Ultralight_Write() OK ");
  Serial.println();
  return 1;
}

void cleanup() {
   for (indexSerial = 0; indexSerial < 20; indexSerial++) {
      inDataSerial[indexSerial] = '\0';
   }
   inCharSerial = '0';
   indexSerial = 0;
   cleanupSerial = false;
}

void doOperations(char *inData) {
  char *realData = inData;
  if (strlen(realData) == 2) {//single command
    if(realData[0] == 'R') { //read operation
      Serial.println("Put a new card");
      Serial.flush();
      int status = readRFID();
      while(status < 0) {
        status = readRFID();
      }
      if (status == 0) {
        Serial.println("To retry read operation send command again");
      }
      return;
    } else if (realData[0] == 'W') { //write operation
      if (listOfCommands.size() == 0) {
        return;
      }
      Serial.println("Start Write to RFID");
      memset(buffer, 0, BUFFER_SIZE);
      char *tmp;
      unsigned int currentPos = 0;
      for(int i = listOfCommands.size(); i > 0; --i) {
        tmp = listOfCommands.getForwardValue();
        currentPos += strlen(tmp);
        Serial.println(tmp);
        if (currentPos > (BUFFER_SIZE - 2)) {
          memset(buffer, 0, BUFFER_SIZE);
          listOfCommands.clear();
          Serial.println("Too big list of commands had been clear");
          return;
        }
        for(int j = 0; j < strlen(tmp); ++j, ++currentPos) {
          buffer[currentPos] = tmp[j];
        }
      }
      for (int i = 0; i < BUFFER_SIZE; ++i)
        Serial.print((char)buffer[i]);
      Serial.println();
      Serial.flush();
      Serial.println("Put a new card");
      Serial.flush();
      int status = writeRFID();
      while(status < 0) {
        status = writeRFID();
      }
      if (status == 0) {
        Serial.println("To retry write operation send command again");
      }
      Serial.println("End Write to RFID");
      Serial.flush();
      return;
    } else if(realData[0] == 'c') { //clear list
      listOfCommands.clear();
      return;
    } else if(realData[0] == 'l') { //list the list
      listOfCommands.reset();
      for (int i = listOfCommands.size(); i > 0; --i) {
        Serial.println(listOfCommands.getForwardValue());
      }
      listOfCommands.reset();
      return;
    }
  } else {
    listOfCommands.addTail(realData);
    return;
  }
}

void readSerialCommand() {
  
   while(Serial.available() > 0) // Don't read unless there you know there is data
   {
      if(indexSerial < 19) // One less than the size of the array
      {
         inCharSerial = Serial.read(); // Read a character
         if (inCharSerial == '\r' || inCharSerial == '\n' || inCharSerial == '\0' || inCharSerial < 35 || inCharSerial > 122) {
            continue;
         }
         inDataSerial[indexSerial++] = inCharSerial; // Store it
         inDataSerial[indexSerial] = '\0'; // Null terminate the string
      } else {
        cleanup();
      }
   }
   if (indexSerial == 0) {
    return;
   }
   if (inDataSerial[indexSerial-1] == '#') {
      doOperations(inDataSerial);
      cleanup();
   }
}

void loop() {
  readSerialCommand();
}
