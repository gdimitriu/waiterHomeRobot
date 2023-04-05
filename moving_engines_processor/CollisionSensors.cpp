/*
 * Moving engines processor - Colission sensors module
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

#include <Wire.h>
#include <PCF8574.h>
#include "CollisionSensors.h"
#include "configuration.h"
#define FRONT_LEFT_MASK 0b00000001
#define FRONT_CENTER_MASK 0b00000010
#define FRONT_RIGHT_MASK 0b00000100
#define REAR_LEFT_MASK 0b00001000
#define REAR_CENTER_MASK 0b00010000
#define REAR_RIGHT_MASK 0b00100000
#define PIN6_MASK 0b01000000
#define PIN7_MASK 0b10000000


bool hasCollision = false;

static PCF8574 expander;

void initCollisionSensors(void) {
  expander.begin(PCF8574_ADDRESS);
  expander.pinMode(0, INPUT_PULLUP); //left front sensor
  expander.pinMode(1, INPUT_PULLUP); //center front sensor
  expander.pinMode(2, INPUT_PULLUP); //right front sensor
  expander.pinMode(4, INPUT_PULLUP); //rear sensor
}

uint8_t readSensors() {  
  return expander.read();
}

bool isFrontLeftCollision(uint8_t sensors) {
  if (( sensors & FRONT_LEFT_MASK ) > 0) {
    return false;
  }
  return true;
}

bool isFrontCenterCollision(uint8_t sensors) {
  if (( sensors & FRONT_CENTER_MASK ) > 0) {
    return false;
  }
  return true;
}

bool isFrontRightCollision(uint8_t sensors) {
  if (( sensors & FRONT_RIGHT_MASK ) > 0) {
    return false;
  }
  return true;
}

bool isRearCenterCollision(uint8_t sensors) {
  if (( sensors & REAR_CENTER_MASK ) > 0) {
    return false;
  }
  return true;
}

bool isRearLeftCollision(uint8_t sensors) {
  if (( sensors & REAR_LEFT_MASK ) > 0) {
    return false;
  }
  return true;
}

bool isRearRightCollision(uint8_t sensors) {
  if (( sensors & REAR_RIGHT_MASK ) > 0) {
    return false;
  }
  return true;
}
