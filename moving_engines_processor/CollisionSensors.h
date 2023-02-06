/*
 * Moving engines processor - Collision Sensors module
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
#ifndef __COLLISION_SENSORS_H__
#define __COLLISION_SENSORS_H__

/**********************************************
 * 
 * init the sensors
 * 
 **********************************************/
extern void initCollisionSensors(void);

/**********************************************
 * 
 * has collision on front left sensor
 * return true if has collision otherwise false
 * 
 **********************************************/
extern bool isFrontLeftCollision(uint8_t sensors);

/**********************************************
 * 
 * has collision on front center sensor
 * return true if has collision otherwise false
 * 
 **********************************************/
extern bool isFrontCenterCollision(uint8_t sensors);

/**********************************************
 * 
 * has collision on front right sensor
 * return true if has collision otherwise false
 * 
 **********************************************/
extern bool isFrontRightCollision(uint8_t sensors);

/**********************************************
 * 
 * has collision on rear center sensor
 * return true if has collision otherwise false
 * 
 **********************************************/
extern bool isRearCenterCollision(uint8_t sensors);

/**********************************************
 * 
 * read the values from sensors array of 8 sensors:
 * 0 is front left
 * 1 is front center
 * 2 is front right
 * 3 is rear left (not used now)
 * 4 is rear center
 * 5 is rear right (not used now
 * 6 not used
 * 7 not used
 * 
 **********************************************/
extern uint8_t readSensors();
#endif
