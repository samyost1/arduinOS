#define STACKSIZE 32

void pushByte(int procID, byte b);
byte popByte(int procID);

// CHAR
void pushChar(int procID, char c);
char popChar(int procID);

float popVal(int procID, int type);

// INT
void pushInt(int procID, int i);
int popInt(int procID);

// FLOAT
void pushFloat(int procID, float f);
float popFloat(int procID);

// STRING
void pushString(int procID, char *s);
char *popString(int procID);

void debugTestStack();