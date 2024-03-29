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

/************************************************************************
 * 
 * CONSTANTS FOR ANDROID AND HARDWARE PLATFORM
 * 
 ************************************************************************/

#define SERIAL_BUFFER 50

#define RxD 8
#define TxD 9

/**********************************************
 * 
 * Address of the PCF874 IO expander
 * 
 **********************************************/
#define PCF8574_ADDRESS 0x21

/**********************************************
 * 
 * Address of the PCA9685 PWM expander
 * 
 **********************************************/
#define PCA9685_ADDRESS 0x40

/**********************************************
 * 
 * enable serial debug
 * (do not use when use the android app)
 * 
 **********************************************/
#define SERIAL_DEBUG_MODE

/**********************************************
 * 
 * left front ecoder pin from arduino
 * 
 **********************************************/
#define LEFT_FRONT_ENCODER_PIN 2

/**********************************************
 * 
 * right front ecoder pin from arduino
 * 
 **********************************************/
#define RIGHT_FRONT_ENCODER_PIN 3

/**********************************************
 * 
 * left back ecoder pin from arduino
 * 
 **********************************************/
#define LEFT_BACK_ENCODER_PIN 4

/**********************************************
 * 
 * right back ecoder pin from arduino
 * 
 **********************************************/
#define RIGHT_BACK_ENCODER_PIN 5

/**********************************************
 * 
 * maximum power for Adafruit_PWMServoDriver
 * 
 **********************************************/
#define ABSOLUTE_MAX_POWER 4095

/**********************************************
 * 
 * right back motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define RIGHT_BACK_MOTOR_PIN2 15

/**********************************************
 * 
 * right back motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define RIGHT_BACK_MOTOR_PIN1 14

/**********************************************
 * 
 * right front motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define RIGHT_FRONT_MOTOR_PIN2 13

/**********************************************
 * 
 * right front motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define RIGHT_FRONT_MOTOR_PIN1 12

/**********************************************
 * 
 * left back motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define LEFT_BACK_MOTOR_PIN2 0

/**********************************************
 * 
 * left back motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define LEFT_BACK_MOTOR_PIN1 1

/**********************************************
 * 
 * left front motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define LEFT_FRONT_MOTOR_PIN2 2

/**********************************************
 * 
 * left front motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
 #define LEFT_FRONT_MOTOR_PIN1 3

/**********************************************
 * 
 * nr of counter to rotate 90
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define COUNT_ROTATE_90 20.0f

/**********************************************
 * 
 * wheel radius in cm
 * 
 **********************************************/
#define WHEEL_RADIUS 8.5f

/**********************************************
 * 
 * nr counter for the encoder wheel
 * 
 **********************************************/
#define ENCODER_WHEEL_STEPS 20

/**********************************************
 * 
 * interrupt pin for collision sensors
 *  
 **********************************************/
#define COLLISION_INTERRUPT_PIN 6

/**********************************************
 * 
 * RFID servo
 *  
 **********************************************/
#define RFID_SERVO_PIN 10

/**********************************************
 * 
 * interrupt pin for RFID UP switch
 *  
 **********************************************/
 #define RFID_UP_SWITCH_PIN 11

/**********************************************
 * 
 * interrupt pin for RFID DOWN switch
 *  
 **********************************************/
 #define RFID_DOWN_SWITCH_PIN 12

/************************************************************************
 * 
 * CONSTANTS AT STARTUP BUT COULD BE MODIFIED USING SERIAL COMMANDS
 * 
 ************************************************************************/
 
/**********************************************
 * 
 * current maximum power
 *  
 **********************************************/
extern int maxPower;

/**********************************************
 * 
 * current power
 *  
 **********************************************/
extern int currentPower;

/**********************************************
 * 
 * minimum power
 *  
 **********************************************/
extern int minPower;

#endif
