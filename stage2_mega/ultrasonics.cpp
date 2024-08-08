/*
 * Stage 2 - ultrasonics operations
 * 
 * Copyright 2024 Gabriel Dimitriu
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
#include <Servo.h>
#include "ultrasonics.h"
#include "configuration.h"

static Servo ultrasonicServo;
static unsigned int currentPos;

unsigned long stopDistance = 10; //cm
unsigned long lowPowerDistance = 20; //cm

void initUltrasonics() {
  ultrasonicServo.attach(ULTRASONIC_SERVO_PIN);
  currentPos = 90;
  ultrasonicServo.write(currentPos);
  pinMode(ULTRASONIC_FRONT_ECHO_PIN, INPUT);                    //Set EchoPin as input, to receive measure result from US-020
  pinMode(ULTRASONIC_FRONT_TRIG_PIN, OUTPUT);                   //Set TrigPin as output, used to send high pusle to trig measurement (>10us)
  pinMode(ULTRASONIC_REAR_ECHO_PIN, INPUT);                    //Set EchoPin as input, to receive measure result from US-020
  pinMode(ULTRASONIC_REAR_TRIG_PIN, OUTPUT);                   //Set TrigPin as output, used to send high pusle to trig measurement (>10us)
}


/**
 * calculate the distance to the object.
 * trigPin the trigger pin
 * echoPin the echo pin
 */
static unsigned long calculateDistance(int trigPin, int echoPin) {
  //set the trigPin to LOW
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  // Sets the trigPin on HIGH state for 10 micro seconds 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  // Reads the echoPin, returns the sound wave travel time in microseconds 
  // Calculating the distance 
  unsigned long Time_Echo_us = pulseIn(echoPin, HIGH);  //calculate the pulse width at EchoPin, 
  if((Time_Echo_us < 60000) && (Time_Echo_us > 1))     //a valid pulse width should be between (1, 60000).
  {
    return ((Time_Echo_us/2)/29.1)*10;
  }
  return 200;
}

unsigned long getFrontDistance(int degree) {
  if ( degree != currentPos) {
    ultrasonicServo.write(degree);
    currentPos = degree;
  }
  return calculateDistance(ULTRASONIC_FRONT_TRIG_PIN,ULTRASONIC_FRONT_ECHO_PIN);
}

unsigned long getRearDistance(int degree) {
  int newPos = 180 - degree;
  if ( newPos != currentPos) {
    ultrasonicServo.write(newPos);
    currentPos = newPos;
  }  
  return calculateDistance(ULTRASONIC_REAR_TRIG_PIN,ULTRASONIC_REAR_ECHO_PIN);
}
