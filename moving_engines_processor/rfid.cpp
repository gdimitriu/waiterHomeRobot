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
 *     RST  -----------  9 (onfigurable, see RST_PIN constant)
 *     3.3V ----------- 3.3V
 *     
 */

#include "rfid.h"
#include "configuration.h"
#include <SPI.h>
#include <MFRC522.h>

MFRC522 *mfrc522;
//Ultraligth mem = 16 pages. 4 bytes per page. 
//Pages 0 to 4 are for special functions.
//it has 36 pages https://www.optimusdigital.ro/ro/wireless-rfid/2548-sticker-nfc-ntag203-alb-rotund-144-bytes.html
static uint8_t pageAddr = 0x05;
static byte buffer[44];//max is 128

void initRFID() {
  mfrc522 = new MFRC522(ssPin,MFRC522::UNUSED_PIN);
  mfrc522->PCD_Init(); // Init MFRC522 card
}
bool isCardPresent() {
  if ( ! mfrc522->PICC_IsNewCardPresent())
    return false;

  // Select one of the cards
  if ( ! mfrc522->PICC_ReadCardSerial())
    return false;
  return true;
}

void readRFID(char *inData) {
    MFRC522::StatusCode status;
    byte size = sizeof(buffer);
    status = (MFRC522::StatusCode) mfrc522->MIFARE_Read(pageAddr, buffer, &size);
    
}
