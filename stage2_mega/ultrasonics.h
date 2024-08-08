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

#ifndef __ULTRASONICS_H__
#define __ULTRASONICS_H__

/**********************************************
 * 
 * initialization
 *  
 **********************************************/
extern void initUltrasonics();

/**********************************************
 * 
 * get front sensor distance from xx degree
 *  
 **********************************************/
 extern unsigned long getFrontDistance(int degree);

 /**********************************************
 * 
 * get rear sensor distance from xx degree
 *  
 **********************************************/
 extern unsigned long getRearDistance(int degree);

 /**********************************************
 * 
 * stop distance
 * (it will make decision to change path)  
 * 
 **********************************************/
 extern unsigned long stopDistance;

/**********************************************
 * 
 * low power distance
 * below this distance it will go with slow down
 * 
 **********************************************/
 extern unsigned long lowPowerDistance;

 #endif
