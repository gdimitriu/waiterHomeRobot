/*
 * Test the sensors using pcf8574 with interrupts.
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
#include<PCF8574.h>
#include <EnableInterrupt.h>

#define FRONT_LEFT_MASK 0b00000001
#define FRONT_CENTER_MASK 0b00000010
#define FRONT_RIGHT_MASK 0b00000100
#define REART_LEFT_MASK 0b00001000
#define REAR_CENTER_MASK 0b00010000
#define REAR_RIGHT_MASK 0b00100000
#define PIN6_MASK 0b01000000
#define PIN7_MASK 0b10000000
#define COLISSION_INTERRUPT_PIN 6

PCF8574 expander;

static bool *collision;

volatile bool hasCollision = false;

static void detectColissionIsr(void) {
  hasCollision = true;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  expander.begin(0x21);
  expander.pinMode(0, INPUT_PULLUP); //left front sensor
  expander.pinMode(1, INPUT_PULLUP); //center front sensor
  expander.pinMode(2, INPUT_PULLUP); //right front sensor
  expander.pinMode(4, INPUT_PULLUP); //rear sensor
  collision = new bool[8];  
  pinMode(COLISSION_INTERRUPT_PIN, INPUT_PULLUP);
  enableInterrupt(COLISSION_INTERRUPT_PIN, detectColissionIsr, FALLING);
}
bool* readSensors() {
  
  uint8_t sensors = expander.read();
  if (( sensors & FRONT_LEFT_MASK) > 0) {
    collision[0] = false;
  } else {
    collision[0] = true;
  }
  if (( sensors & FRONT_CENTER_MASK) > 0) {
    collision[1] = false;
  } else {
    collision[1] = true;
  }
  if (( sensors & FRONT_RIGHT_MASK) > 0) {
    collision[2] = false;
  } else {
    collision[2] = true;
  }
  if (( sensors & REAR_CENTER_MASK) > 0) {
    collision[4] = false;
  } else {
    collision[4] = true;
  }
  collision[3] = false;
  for (int i = 5; i < 8; i++) {
    collision[i] = false;
  }
  return collision;
}

void loop() {
  
  while(hasCollision == false)
  // loop until detect
  ;
  readSensors();
  Serial.print("front left=");Serial.println(collision[0]);
  Serial.print("front center=");Serial.println(collision[1]);
  Serial.print("front right=");Serial.println(collision[2]);
  Serial.print("rear center=");Serial.println(collision[4]);
  hasCollision = false;
}
