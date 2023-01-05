/*
 * StringList.cpp
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
#include "StringList.h"

StringListNode::StringListNode(char *value, StringListNode* prev, StringListNode* next) {
  this->value = value;
  this->previous = prev;
  this->next = next;
}

StringListNode::StringListNode(char *value) {
  this->value = value;
  this->previous = NULL;
  this->next = NULL;
}

StringListNode::~StringListNode() {
  delete []value;
}

StringListNode* StringListNode::getPrevious() {
  return previous;
}

StringListNode* StringListNode::getNext() {
  return next;
}

void StringListNode::setPrevious(StringListNode* prev) {
  this->previous = prev;
}

void StringListNode::setNext(StringListNode* next) {
  this->next = next;
}

char* StringListNode::getValue() {
  return value;
}

StringList::StringList() {
  tail = head = current = NULL;
  isAtEnd = false;
  _size = 0;
}

void StringList::clear() {
  if (head == NULL && tail == NULL) {
    return;
  }
  StringListNode* tmp = tail->getPrevious();
  while(tmp != NULL) {
    delete tail;
    tail = tmp;
    tmp = tail->getPrevious();
  }
  tail = head = current = NULL;
  isAtEnd = false;
  _size = 0;
}

void StringList::addTail(StringListNode *element) {
  _size++;
  if (head == NULL && tail == NULL) {
    head = tail = element;
    element->setPrevious(NULL);
    element->setNext(NULL);
    return;
  }
  if (head == tail) {
    element->setPrevious(tail);
    head->setNext(element);
    tail = element;
    return;
  }
  tail->setNext(element);
  element->setPrevious(tail);
  element->setNext(NULL);
  tail = element;
}


void StringList::addTail(char *value) {
  char *temp = new char[strlen(value)+1];
  memcpy(temp,value,strlen(value)+1);
  StringListNode * elem = new StringListNode(temp);
  addTail(elem);
}

void StringList::reset() {
  current = NULL;
  isAtEnd = false;
}

char *StringList::getForwardValue() {
  char* ret;
  if (head == NULL && tail == NULL)
    return NULL;
  if (isAtEnd)
    return NULL;
  if (current == NULL) {
    ret = head->getValue();
    current = head->getNext();
    if (current == NULL)
      isAtEnd = true;
  } else {
    ret = current->getValue();
    current = current->getNext();
    if (current == NULL)
      isAtEnd = true;
  }
  return ret;
}

char *StringList::getReverseValue() {
  char* ret;
  if (head == NULL && tail == NULL)
    return NULL;
  if (isAtEnd)
    return NULL;
  if (current == NULL) {
    ret = tail->getValue();
    current = tail->getPrevious();
    if (current == NULL)
      isAtEnd = true;
  } else {
    ret = current->getValue();
    current = current->getPrevious();
    if (current == NULL)
      isAtEnd = true;
  }
  return ret;
}

unsigned int StringList::size() {
  return _size;
}
