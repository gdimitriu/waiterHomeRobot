/*
 * Moving engines processor - Cnfiguration for the platform
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
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#define SERIAL_BUFFER 50

/**********************************************
 * left front ecoder pin from arduino
 * 
 **********************************************/
extern unsigned int leftFrontEncoderPin;

/**********************************************
 * right front ecoder pin from arduino
 * 
 **********************************************/
extern unsigned int rightFrontEncoderPin;

/**********************************************
 * left back ecoder pin from arduino
 * 
 **********************************************/
extern unsigned int leftBackEncoderPin;

/**********************************************
 * right back ecoder pin from arduino
 * 
 **********************************************/
extern unsigned int rightBackEncoderPin;

/**********************************************
 * left front motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern unsigned int leftFrontMotorPin1;

/**********************************************
 * left front motor pin  2
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern unsigned int leftFrontMotorPin2;

/**********************************************
 * right front motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern unsigned int rightFrontMotorPin1;

/**********************************************
 * right front motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern unsigned int rightFrontMotorPin2;

/**********************************************
 * left back motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern unsigned int leftBackMotorPin1;

/**********************************************
 * left back motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern unsigned int leftBackMotorPin2;

/**********************************************
 * right back motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern unsigned int rightBackMotorPin1;

/**********************************************
 * right back motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern unsigned int rightBackMotorPin2;

/**********************************************
 * nr of counter to rotate 90 left
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern int countRotate90Left;

/**********************************************
 * nr of counter to rotate 90 right
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern int countRotate90Right;

/**********************************************
 * nr of counter to rotate inner wheel
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
 
extern int countRotate1Inner;
/**********************************************
 * nr of counter to rotate outer wheel
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern int countRotate1Outer;

extern int maxPower;
extern int currentPower;
extern int minPower;

/**********************************************
 * wheel radius in cm
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern float wheelRadius;

/**********************************************
 * nr counter for the encoder wheel
 * this is the pin from Adafruit_PWMServoDriver. 
 **********************************************/
extern int encoderWheelSteps;


/**********************************************
 * if has rfid MFRC522
 *  
 **********************************************/
extern bool hasRFID;

/**********************************************
 * ss pin for rfid
 *  
 **********************************************/
extern unsigned int ssPin;
#endif
