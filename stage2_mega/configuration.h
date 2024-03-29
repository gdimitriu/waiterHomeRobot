/*
 * Stage 2 - configuration
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

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__
/**********************************************************************
 *
 * Flags to activate standalone tests for components
 *
 **********************************************************************/
/**********************************************
 * 
 * uncomment this to have LCD test
 * 
 **********************************************/
//#define TEST_LCD

/*********************************************
 * 
 * uncomment this to have read from SD and
 * print on LCD test
 * data are into tests/sd_lcd.txt
 * 
 *********************************************/
//#define TEST_SD_LCD

/*********************************************************************
 * 
 * Real Functionality
 * 
 *********************************************************************/
/**********************************************
 * DC from ILI9341 pin
 * 
 **********************************************/
#define DC_LCD 23

/**********************************************
 * RESET from ILI9341 pin
 * 
 **********************************************/
#define RST_LCD 22

/**********************************************
 * CS from ILI9341 pin
 * unused on my board because it does not have it
 * choose something that is not used
 **********************************************/
#define CS_LCD 26

/**********************************************
 * 
 * if has rfid SD CARD READER
 *  
 **********************************************/
//#define HAS_SD

/**********************************************
 * 
 * CS from SD board pin
 * 
 **********************************************/
#define CHIP_SELECT_SD 25

/**********************************************
 * 
 * CD from SD board pin
 * 
 **********************************************/
#define CARD_DETECT_SD 24

/**********************************************
 * 
 * Speaker PIN
 * 
 **********************************************/
#define LEFT_SPEAKER_PIN 10

/**********************************************
 * 
 * if has rfid MFRC522
 *  
 **********************************************/
//#define HAS_RFID

/**********************************************
 * 
 * ss pin for rfid
 *  
 **********************************************/
#define SS_RFID_PIN 27

#endif
