/*
 * Stage 2 - SD card operation
 * 
 * This is used only for LCD and other operations not for Sound Module.
 * 
 * Copyright 2023 Gabriel Dimitriu* 
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

#ifndef __SD_OPERATIONS_H__
#define __SD_OPERATIONS_H__
#include <SD.h>
#include "configuration.h"

/**********************************************
 * 
 * initialize the SD Card Reader
 * 
 **********************************************/
extern void initSDCardReader();

/**********************************************
 * 
 * open the file which should be 8.3
 * 
 **********************************************/
extern bool openFile(char *fileName);


/**********************************************
 * 
 * open for write the file which should be 8.3
 * if the file exist it will be deleted
 * 
 **********************************************/
extern bool openWriteFile(char *fileName);

/**********************************************
 * 
 * close file
 * 
 **********************************************/
extern void closeFile();

/**********************************************
 * 
 * remove file
 * 
 **********************************************/
extern void removeFile(char *fileName);
/**********************************************
 * 
 * read next line from the opened file
 * 
 **********************************************/
extern char* readNextLine();

/**********************************************
 * 
 * write next line in the  opened file
 * 
 **********************************************/
extern void writeNextLine(char* value);

/**********************************************
 * 
 * read full
 * 
 **********************************************/
extern void readFullFile(char *buffer, unsigned int size);

/**********************************************************************
 *
 * standalone tests enabled by TEST_SD_LCD flag
 *
 **********************************************************************/
#ifdef TEST_SD_LCD
  extern void printInSetupSdLcdTest();
#endif

/**********************************************
 * 
 * 
 * play fileName
 * 
 **********************************************/
extern void playSound(char *fileName);
#endif
