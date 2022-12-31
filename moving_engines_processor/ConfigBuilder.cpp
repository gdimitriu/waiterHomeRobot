/*
 * Moving engines processor - configuration builder
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
#include "ConfigBuilder.h"
#include "configuration.h"

unsigned int leftFrontEncoderPin = 5;
unsigned int leftBackEncoderPin = 4;
unsigned int rightFrontEncoderPin = 3;
unsigned int rightBackEncoderPin = 2;
unsigned int ssPin = 10;

unsigned int leftFrontMotorPin1 = 14;
unsigned int leftFrontMotorPin2 = 15;
unsigned int rightFrontMotorPin1 = 13;
unsigned int rightFrontMotorPin2 = 12;
unsigned int leftBackMotorPin1 = 0;
unsigned int leftBackMotorPin2 = 1;
unsigned int rightBackMotorPin1 = 2;
unsigned int rightBackMotorPin2 = 3;


int countRotate90Left = 1;
int countRotate90Right = 1;
int countRotate1Inner = 1;
int countRotate1Outer = 1;

bool hasRFID = false;

bool hasColissionSensors = false;
unsigned int colissionInterruptPin = 6;
