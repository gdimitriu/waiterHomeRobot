/*
 * Stage 2 - Power monitoring
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

#ifndef __POWER_MONITORING_H__
#define __POWER_MONITORING_H__
#include "configuration.h"
#include <Arduino.h>

/*
 * initialize timers for power monitoring
 */
extern void initPowerMonitoring();

/*
 * get the power level as volts
 */
extern float getPowerLevel();

/*
 * get the power level as bar
 */
extern uint8_t getPowerLineBar();

extern void usePowerLevelAtTimeout();

#endif
