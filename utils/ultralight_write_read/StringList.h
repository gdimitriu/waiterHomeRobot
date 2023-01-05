/*
 * StringList.h
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
#ifndef STRING_LIST_H
#define STRING_LIST_H
#include <Arduino.h>

class StringListNode {
public:
  StringListNode(char *value, StringListNode* prev, StringListNode* next);
  StringListNode(char *value);
  ~StringListNode();
  void setPrevious(StringListNode* prev);
  void setNext(StringListNode* next);
  StringListNode* getPrevious();
  StringListNode* getNext();
  char* getValue();
private :
  char* value;
  StringListNode* previous;
  StringListNode* next;
};

class StringList {
public:
  StringList();
  void addTail(StringListNode *);
  void addTail(char* value);
  char* getForwardValue();
  char* getReverseValue();
  void reset();
  void clear();
  unsigned int size();
private:
  bool isAtEnd;
  unsigned int _size;
  StringListNode *current;
  StringListNode *head;
  StringListNode *tail;
};
#endif
