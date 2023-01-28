/*
 * Moving engines processor - Move engines with encoder and sensors
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

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <EnableInterrupt.h>
#include <NeoSWSerial.h>
#include "configuration.h"
#include "MoveEngines.h"
#include "ColissionSensors.h"

extern Adafruit_PWMServoDriver pwmDriver; 

/**********************************************
 * if has collision if true else false
 * 
 **********************************************/
static volatile bool hasCollision = false;

static volatile uint16_t  left_front_encoder_count = 0;
static volatile uint16_t  right_front_encoder_count = 0;
static volatile uint16_t  left_back_encoder_count = 0;
static volatile uint16_t  right_back_encoder_count = 0;

static float PPI_front_left;
static float PPI_front_right;
static float PPI_back_left;
static float PPI_back_right;
static float currentLeftFrontPosition = 0.0f;
static float currentRightFrontPosition = 0.0f;
static float currentLeftBackPosition = 0.0f;
static float currentRightBackPosition = 0.0f;
bool encoderEnabled = false;

static float distances[4];

void neoSSerial1ISR()
{
    NeoSWSerial::rxISR(*portInputRegister(digitalPinToPort(RxD)));
}

/*
 * isr for encoder pins
 */
static void isrLeftFrontEncoder() {
  left_front_encoder_count++;
}

static void isrRightFrontEncoder() {
  right_front_encoder_count++;
}

static void isrLeftBackEncoder() {
  left_back_encoder_count++;
}

static void isrRightBackEncoder() {
  right_back_encoder_count++;
}

void resetCounters() {
  left_front_encoder_count = 0;
  right_front_encoder_count = 0;
  left_back_encoder_count = 0;
  right_back_encoder_count = 0;
}

void disableEncoders() {
  if (!encoderEnabled)
    return;
  disableInterrupt(LEFT_FRONT_ENCODER_PIN);
  disableInterrupt(RIGHT_FRONT_ENCODER_PIN);
  disableInterrupt(LEFT_BACK_ENCODER_PIN);
  disableInterrupt(RIGHT_BACK_ENCODER_PIN);  
  encoderEnabled = false;
}

void enableEncoders() {
  if (encoderEnabled)
    return;
  enableInterrupt(LEFT_FRONT_ENCODER_PIN, isrLeftFrontEncoder, RISING);
  enableInterrupt(RIGHT_FRONT_ENCODER_PIN, isrRightFrontEncoder, RISING);
  enableInterrupt(LEFT_BACK_ENCODER_PIN, isrLeftBackEncoder, RISING);
  enableInterrupt(RIGHT_BACK_ENCODER_PIN, isrRightBackEncoder, RISING);
  encoderEnabled = true;
  resetCounters();
}

static void detectColissionIsr(void) {
  hasCollision = true;
}

void engineSetup() {
  PPI_front_left = ENCODER_WHEEL_STEPS/(2*PI*WHEEL_RADIUS);
  PPI_front_right = ENCODER_WHEEL_STEPS/(2*PI*WHEEL_RADIUS);
  PPI_back_left = ENCODER_WHEEL_STEPS/(2*PI*WHEEL_RADIUS);
  PPI_back_right = ENCODER_WHEEL_STEPS/(2*PI*WHEEL_RADIUS);
  //enable encoders ISR
  pinMode(LEFT_FRONT_ENCODER_PIN, INPUT_PULLUP);
  pinMode(RIGHT_FRONT_ENCODER_PIN, INPUT_PULLUP);
  pinMode(LEFT_BACK_ENCODER_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BACK_ENCODER_PIN, INPUT_PULLUP);
  encoderEnabled = false;
  //enable interrupt sensors
#ifdef HAS_COLLISION_SENSORS  
    pinMode(COLISSION_INTERRUPT_PIN, INPUT_PULLUP);
    enableInterrupt(COLISSION_INTERRUPT_PIN, detectColissionIsr, FALLING);
#endif
  //enable communication ... all enable interrupt should be in same cpp file
  enableInterrupt(RxD, neoSSerial1ISR, CHANGE);
}

uint16_t getLeftFrontEncoderCount() {
  return left_front_encoder_count;
}

uint16_t getRightFrontEncoderCount() {
  return right_front_encoder_count;
}

uint16_t getLeftBackEncoderCount() {
  return left_back_encoder_count;
}

uint16_t getRightBackEncoderCount() {
  return right_back_encoder_count;
}

void stopLeftEngines() {
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN1, 0, 4095);
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN2, 0, 4095);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN1, 0, 4095);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN2, 0, 4095);
}

void stopRightEngines() {
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN1, 0, 4095);
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN2, 0, 4095);
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN1, 0, 4095);
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN2, 0, 4095);
}
void breakAllEngines() {
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN1, 0, 4095);
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN2, 0, 4095);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN1, 0, 4095);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN2, 0, 4095);
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN1, 0, 4095);
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN2, 0, maxPower);    
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN1, 0, maxPower);
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN2, 0, maxPower);
}
/*
* Move the platform in predefined directions.
*/
void go(int speedLeft, int speedRight) {

  if (speedLeft == 0 && speedRight == 0 ) {
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN1, 0, 0);
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN2, 0, 0);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN1, 0, 0);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN2, 0, 0);
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN1, 0, 0);
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN2, 0, 0);
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN1, 0, 0);
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN2, 0, 0);
    return;
  }
  if (speedLeft > 0) {
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN1, 0, speedLeft);
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN2, 0, 0);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN1, 0, speedLeft);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN2, 0, 0);
  } 
  else {
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN1, 0, 0);
    pwmDriver.setPWM(LEFT_FRONT_MOTOR_PIN2, 0, -speedLeft);    
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN1, 0, 0);
    pwmDriver.setPWM(LEFT_BACK_MOTOR_PIN2, 0, -speedLeft);    
  }
 
  if (speedRight > 0) {
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN1, 0, speedRight);
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN2, 0, 0);    
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN1, 0, speedRight);
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN2, 0, 0);
  }else {
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN1, 0, 0);    
    pwmDriver.setPWM(RIGHT_FRONT_MOTOR_PIN2, 0, -speedRight);    
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN1, 0, 0);
    pwmDriver.setPWM(RIGHT_BACK_MOTOR_PIN2, 0, -speedRight);
  }
}


void moveLinear(float distance) {
  float targetDistance = distance;
  currentLeftFrontPosition = 0.0f;
  currentRightFrontPosition = 0.0f;
  currentLeftBackPosition = 0.0f;
  currentRightBackPosition = 0.0f;

  if (targetDistance > 0) {
    go(currentPower,currentPower);
  } else if (targetDistance < 0) {
    go(-currentPower,-currentPower);
    targetDistance = -targetDistance;
  } else {
    return;
  }
  boolean stopLeft = false;
  boolean stopRight = false;
  while(!stopLeft || !stopRight){
    if (hasCollision == true) {
      breakAllEngines();
      bool *sensors = readSensors();
      if (sensors[0] == true || sensors[1] == true || sensors[2] == true) {
        //front sensors detect object
        if (distance < 0) { //resume
          go(-currentPower,-currentPower);
          hasCollision = false;
        } else {
          stopLeft = true;
          stopRight = true;
        }
      } else if (sensors[4] == true || sensors[3] == true || sensors[5] == true) {
        //rear sensors detect object
        if (distance < 0) { //resume
          go(currentPower,currentPower);
          hasCollision = false;
        } else {
          stopLeft = true;
          stopRight = true;
        }
      } else {
        if (distance > 0) {
          go(currentPower,currentPower);
          hasCollision = false;
        } else if (distance < 0) {
          go(-currentPower,-currentPower);
          hasCollision = false;
        }
      }
      
    }
    if (!stopLeft) {
      if(((targetDistance - currentLeftFrontPosition) > 0.2)) {
         currentLeftFrontPosition = left_front_encoder_count/PPI_front_left;
      } else {
        stopLeftEngines();
        stopLeft = true;
      }
    }
    if (!stopRight) {
      if (((targetDistance - currentRightFrontPosition) > 0.2)) {
        currentRightFrontPosition = right_front_encoder_count/PPI_front_right;
      } else {
        stopRightEngines();
        stopRight=true;
      }
    }
  }
  breakAllEngines();
  delay(100);
  go(0,0);
}

void rotateDegree(long nr) {
  if (nr < 0) {
    go(-currentPower,currentPower);
    nr = -nr;
    while(left_front_encoder_count < COUNT_ROTATE_INNER*nr && right_front_encoder_count < COUNT_ROATE_OUTER*nr && hasCollision == false);
  } else if (nr > 0) {
    go(currentPower,-currentPower);
    while(left_front_encoder_count < COUNT_ROATE_OUTER*nr && right_front_encoder_count < COUNT_ROTATE_INNER*nr && hasCollision == false);
  } else {
    return;
  }
  breakAllEngines();
  delay(100);  
  go(0,0);
}

bool isInterrupted() {
  return hasCollision;
}

void resetInterrupt() {
  hasCollision = false;
}

float* getCurrentDistances() {  
  distances[0] = currentLeftFrontPosition;
  distances[1] = currentRightFrontPosition;
  distances[2] = currentLeftBackPosition;
  distances[3] = currentRightBackPosition;
  return distances;
}
