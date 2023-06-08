#include <Arduino.h>
void pushByte(byte b);
byte popByte();

// CHAR
void pushChar(char c);
char popChar();

float popVal();

// INT
void pushInt(int i);
int popInt();

// FLOAT
void pushFloat(float f);
float popFloat();

// STRING
void pushString(char *s);
char *popString();

void debugTestStack();