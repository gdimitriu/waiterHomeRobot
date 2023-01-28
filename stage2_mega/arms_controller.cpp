/*
 * Stage 2 - arms controller
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

#include "arms_controller.h"
#include <Arduino.h>
#include <Servo.h>

#define SHOULDER_ORIG_POS 0
#define ELBOW_ORIG_POS 180
#define WRIST_ORIG_POS 0
#define GRIPPER_ORIG_POS 0

#define ARM_DELAY 100
/**********************************************
 * left arm servo and position
 * 
 **********************************************/
static Servo leftServoGripper;
static Servo leftServoElbow;
static Servo leftServoWrist;
static Servo leftServoShoulder;

static int currentLeftShoulderPos = SHOULDER_ORIG_POS;
static int currentLeftElbowPos = ELBOW_ORIG_POS;
static int currentLeftWristPos = WRIST_ORIG_POS;
static int currentLeftGripperPos = GRIPPER_ORIG_POS;

/**********************************************
 * right arm servo and position
 * 
 **********************************************/
static Servo rightServoGripper;
static Servo rightServoElbow;
static Servo rightServoWrist;
static Servo rightServoShoulder;

static int currentRightShoulderPos = SHOULDER_ORIG_POS;
static int currentRightElbowPos = ELBOW_ORIG_POS;
static int currentRightWristPos = WRIST_ORIG_POS;
static int currentRightGripperPos = GRIPPER_ORIG_POS;

void initArmsController() {
   pinMode(RELAY_ARMS_PIN, OUTPUT);
}

void attachServos() {
  digitalWrite(RELAY_ARMS_PIN, 1);
  //reset current position
  currentLeftShoulderPos = SHOULDER_ORIG_POS;
  currentLeftElbowPos = ELBOW_ORIG_POS;
  currentLeftWristPos = WRIST_ORIG_POS;
  currentLeftGripperPos = GRIPPER_ORIG_POS;
  currentRightShoulderPos = SHOULDER_ORIG_POS;
  currentRightElbowPos = ELBOW_ORIG_POS;
  currentRightWristPos = WRIST_ORIG_POS;
  currentRightGripperPos = GRIPPER_ORIG_POS;
  
  //attach servos and put into original position
  leftServoShoulder.attach(LEFT_ARM_SHOULDER);
  leftServoShoulder.write(currentLeftShoulderPos);
  rightServoShoulder.attach(RIGHT_ARM_SHOULDER);
  rightServoShoulder.write(currentRightShoulderPos);
  leftServoElbow.attach(LEFT_ARM_ELBOW);
  leftServoElbow.write(currentLeftElbowPos);
  rightServoElbow.attach(RIGHT_ARM_ELBOW);
  rightServoElbow.write(currentRightElbowPos);
  leftServoWrist.attach(LEFT_ARM_WRIST);
  leftServoWrist.write(currentLeftWristPos);
  rightServoWrist.attach(RIGHT_ARM_WRIST);
  rightServoWrist.write(currentRightWristPos);
  leftServoGripper.attach(LEFT_ARM_GRIPPER);
  leftServoGripper.write(currentLeftGripperPos);
  rightServoGripper.attach(RIGHT_ARM_GRIPPER);
  rightServoGripper.write(currentRightGripperPos);
}

void detachServos() {
  leftServoShoulder.detach();
  rightServoShoulder.detach();
  leftServoElbow.detach();
  rightServoElbow.detach();
  leftServoWrist.detach();
  rightServoWrist.detach();
  leftServoGripper.detach();
  rightServoGripper.detach();
  digitalWrite(RELAY_ARMS_PIN, 0);
}

bool moveBothHorizontally(uint8_t target) {
  if (target < 0 || target > 180) {
    return false;
  }
  if ( currentLeftShoulderPos < target) {
    for (; currentLeftShoulderPos <= target && currentRightShoulderPos <= target; currentLeftElbowPos--, currentRightElbowPos --, currentLeftShoulderPos++, currentRightShoulderPos) {
      leftServoShoulder.write(currentLeftShoulderPos);
      rightServoShoulder.write(currentRightShoulderPos);
      leftServoElbow.write(currentLeftElbowPos);
      rightServoElbow.write(currentRightElbowPos);
      delay(ARM_DELAY);
    }
  } else {
    for (; currentLeftShoulderPos >= target && currentRightShoulderPos >= target; currentLeftElbowPos++, currentRightElbowPos++, currentLeftShoulderPos--, currentRightShoulderPos-- ) {
      leftServoShoulder.write(currentLeftShoulderPos);
      rightServoShoulder.write(currentRightShoulderPos);
      leftServoElbow.write(currentLeftElbowPos);
      rightServoElbow.write(currentRightElbowPos);
      delay(ARM_DELAY);
    }
  }
  return true;
}
