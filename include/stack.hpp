#ifndef STACK_H
#define STACK_H

#include <Arduino.h>
#include <process.hpp>

const int STACKSIZE = 16;

void pushByte(int procID, int& sp, byte b);
byte popByte(int procID, int& sp);

// CHAR
void pushChar(int procID, int& sp, char c);
char popChar(int procID, int& sp);

float popVal(int procID, int& sp, int type);

// INT
void pushInt(int procID, int& sp, int i);
int popInt(int procID, int& sp);

// FLOAT
void pushFloat(int procID, int& sp, float f);
float popFloat(int procID ,int& sp);

// STRING
void pushString(int procID, int& sp,char *s);
char *popString(int procID, int& sp);

void debugTestStack();

#endif  // STACK_H