/*
 *  Stage 2 - path navigation module
 * 
 * string_list.h (a string list that is used to store path commands)
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

#include <Arduino.h>
#include "sd_operations.h"

struct node {
  struct node *previous;
  struct node *next;
  char *data;
};
typedef struct node string_list_node;

static string_list_node *beginCommandList = NULL;
static string_list_node *endCommandList = NULL;
static string_list_node *currentPosInList = NULL;

static string_list_node *allocate(void) {
  string_list_node * theNode;
  theNode = new string_list_node;
  theNode-> next = NULL;
  theNode->previous = NULL;
  theNode->data = NULL;
  return theNode;
}

string_list_node *getNext(string_list_node *theNode) {
  return theNode->next;
}

string_list_node *getPrevious(string_list_node *theNode) {
  return theNode->previous;
}

bool isAtBeginig(string_list_node *theNode) {
  if ( theNode->previous == NULL )
    return true;
  return false;
}

bool isAtEnd(string_list_node *theNode) {
  if ( theNode->next == NULL ) 
    return true;
  return false;
}

void clearList() {
  if ( beginCommandList == NULL ) {
    return;
  }
  string_list_node *current = beginCommandList;
  while ( beginCommandList != NULL ) {
    current = beginCommandList;
    beginCommandList = beginCommandList->next;
    delete current->data;
    delete current;
  }
  beginCommandList = NULL;
  currentPosInList = NULL;
  endCommandList = NULL;
}

void addCommand(char *data) {
  string_list_node *current = allocate();
  current->data = new char(strlen(data) + 1);
  memcpy(data, current->data, (strlen(data) + 1)*sizeof(char));
  if ( beginCommandList == NULL ) {
    beginCommandList = current;
    endCommandList = current;
    currentPosInList = current;
    return;
  }
  current->previous = endCommandList;
  endCommandList->next = current;
  endCommandList = current;
}

void loadFileInMemory(char *file) {
  if ( openFile(file) ) {
    clearList();
    char *val = readNextLine();
    if ( val != NULL ) {
      addCommand(val);
    }
  }
}


void saveFileFromMemory(char *file) {
  if ( openWriteFile(file) ) {
    currentPosInList = beginCommandList;
    while ( currentPosInList != NULL ) {
      writeNextLine(currentPosInList->data);
      currentPosInList = currentPosInList->next;
    }
  }
}

void setPathDirection(bool direction) {
  if (direction) {
    currentPosInList = beginCommandList;
  } else {
    currentPosInList = endCommandList;
  }
}

char* getNextCommand() {
  if ( currentPosInList == NULL ) {
    return NULL;
  }
  char * ret = currentPosInList->data;
  currentPosInList = currentPosInList->next;
  return ret;
}

char* getPreviousCommand() {
  if ( currentPosInList == NULL ) {
    return NULL;
  }
  char * ret = currentPosInList->data;
  currentPosInList = currentPosInList->previous;
}
