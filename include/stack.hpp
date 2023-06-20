#include <Arduino.h>
#define STACKSIZE 32
// const int STACKSIZE;
byte stack[STACKSIZE];

void pushByte(byte b);
byte popByte();

// CHAR
void pushChar(char c);
char popChar();

float popVal(int type);

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