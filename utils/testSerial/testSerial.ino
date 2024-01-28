/*
 * test Serial moving engine processor
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

#include <SoftwareSerial.h>

#define RxD 8
#define TxD 9

SoftwareSerial BTSerial(RxD, TxD);

void setup() {
  Serial.begin(38400);
  BTSerial.begin(38400);
  Serial.println("Starting ...");
}

void loop() {
  if (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  if (Serial.available()) 
    BTSerial.write(Serial.read());
}
