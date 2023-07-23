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

#ifndef __ARMS_CONTROLLER_H__
#define __ARMS_CONTROLLER_H__
#include <Arduino.h>
#include "configuration.h"

/**********************************************
 * 
 * initialize the arms controller
 * 
 **********************************************/
extern void initArmsController();

/**********************************************
 * 
 * attach arms controller
 * 
 **********************************************/
extern void attachServos();

/**********************************************
 * 
 * detach arms controller
 * 
 **********************************************/
extern void detachServos();

/**********************************************
 * 
 * move both arms slow with payload horizontally
 * 
 **********************************************/
extern bool moveBothHorizontally(uint8_t target);

#endif
