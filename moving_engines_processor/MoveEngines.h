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

#ifndef __MOVEENGINES_H__ 
#define __MOVEENGINES_H__ 
/**********************************************
 * move linear with distance
 * 
 **********************************************/
 
void moveLinear(float distance);
/**********************************************
 * rotate the platform with degree
 * negative values are for left
 * positive values are for right
 * 
 **********************************************/
void rotateDegree(long nr);

/**********************************************
 * move all engines with left and right speed
 * 
 **********************************************/
void go(int speedLeft, int speedRight);

/**********************************************
 * break all engines
 * 
 **********************************************/
void breakAllEngines();

/**********************************************
 * reset encoder counters
 * 
 **********************************************/
void resetCounters();

/**********************************************
 * return the left front encoder count
 * 
 **********************************************/
uint16_t getLeftFrontEncoderCount();

/**********************************************
 * return the right front encoder count
 * 
 **********************************************/
uint16_t getRightFrontEncoderCount();

/**********************************************
 * return the left back encoder count
 * 
 **********************************************/
uint16_t getLeftBackEncoderCount();

/**********************************************
 * return the right back encoder count
 * 
 **********************************************/
uint16_t getRightBackEncoderCount();

/**********************************************
 * enable encoders for move with distance
 * 
 **********************************************/
void enableEncoders();

/**********************************************
 * disable encoders
 * 
 **********************************************/
void disableEncoders();

/**********************************************
 * setup engines, encoders and sensors
 * 
 **********************************************/
void engineSetup();

#endif