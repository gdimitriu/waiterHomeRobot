/*
 * Engine calibration wihtout encoder
 * i2c pwm expander PCA9685.
 * 
 * Copyright 2023 Gabriel Dimitriu
 *
 * This file is part of waiterHomeRobot

 * waiterHomeRobot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
  
 * waiterHomeRobot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
  
 * You should have received a copy of the GNU General Public License
 * along with waiterHomeRobot; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/**********************************************
 * 
 * left front motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define LEFT_FRONT_MOTOR_PIN1 14

/**********************************************
 * 
 * left front motor pin  2
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define LEFT_FRONT_MOTOR_PIN2 15

/**********************************************
 * 
 * right front motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define RIGHT_FRONT_MOTOR_PIN1 13

/**********************************************
 * 
 * right front motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define RIGHT_FRONT_MOTOR_PIN2 12

/**********************************************
 * 
 * left back motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define LEFT_BACK_MOTOR_PIN1 0

/**********************************************
 * 
 * left back motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define LEFT_BACK_MOTOR_PIN2 1

/**********************************************
 * 
 * right back motor pin 1
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
#define RIGHT_BACK_MOTOR_PIN1 2

/**********************************************
 * 
 * right back motor pin 2
 * this is the pin from Adafruit_PWMServoDriver. 
 * 
 **********************************************/
 #define RIGHT_BACK_MOTOR_PIN2 3

int maximumPowerLevel = 4000;

char inData[20]; // Allocate some space for the string
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character

Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver(0x40);

void printMenu() {
  Serial.println( "--------------------------------------------------" );
  Serial.println( "Calibration of engines");
  Serial.println( "--------------------------------------------------" );
  Serial.println( "f# forward" );
  Serial.println( "b# backward" );
  Serial.println( "s# stop all" );
  Serial.println( "-----------------------------" );
}

void makeCleanup() {
  for (index = 0; index < 20; index++) {
    inData[index] = '\0';
  }
  index = 0;
  inChar ='\0';
  Serial.flush();
}

void setup() {
  Serial.begin(9600);
  pwmDriver.begin();
  pwmDriver.setOscillatorFrequency(27000000);
  pwmDriver.setPWMFreq(50.0);
  Wire.setClock(400000);
  printMenu();
  makeCleanup();
}

boolean isValidNumber(char *data, int size)
{
  if (size == 0 ) return false;
   boolean isNum=false;
   if(!(data[0] == '+' || data[0] == '-' || isDigit(data[0]))) return false;

   for(byte i=1;i<size;i++)
   {
       if(!(isDigit(data[i]) || data[i] == '.')) return false;
   }
   return true;
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

void makeMove() {
   if (inData[0] == 'f') {
      Serial.println("forward");
      go(maximumPowerLevel, maximumPowerLevel);
    } else if (inData[0] == 'b') {
      Serial.println("bacward");
      go(-maximumPowerLevel, -maximumPowerLevel);
    } else if (inData[0] == 's') {
      Serial.println("stop all");
      go(0,0);
    } else {
      printMenu();
    }
}

void loop() {
  while(Serial.available() > 0) // Don't read unless there you know there is data
  {
     if(index < 19) // One less than the size of the array
     {
        inChar = Serial.read(); // Read a character
        if (inChar=='\r' || inChar=='\n' || inChar =='\0' || inChar < 35 || inChar > 122) {
          continue;
        }
        //commands start with a letter capital or small
        if (index == 0 && !((inChar >64 && inChar <91) || (inChar > 96 && inChar<123))) {
          continue;
        }    
        inData[index++] = inChar; // Store it
        inData[index] = '\0'; // Null terminate the string
        if (inChar == '#') {
          break;
        }
     } else {
        makeCleanup();
     }
  }
  if (index >= 1) {
    if (inData[index - 1] == '#') {
      makeMove();
      makeCleanup();
    }
  }
}
