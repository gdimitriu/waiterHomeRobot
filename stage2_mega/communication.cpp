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

#define SIZE_BUFFER_ENGINES 50
static char inDataEngines[SIZE_BUFFER_ENGINES];

void initCommunications() {
  //initialize serial to moving engine processor
  Serial2.begin(38400);
  //initialize serial to brain or BLE if had no brain
  Serial3.begin(38400);
}

void receiveCommand() {
  if (Serial2.available() > 0 || Serial3.available() > 0) {
    //order comes from BLE or brain
    if (Serial3.available() > 0) {
      
    }
  }
}

void sendCommandToEngines(char *buffer) {
  
}
