#include <Arduino.h>

#include <process.hpp>
#include <stack.hpp>

byte stack[PROCESS_TABLE_SIZE][STACKSIZE];
byte sp = 0;

void pushByte(int procID, byte b) { stack[procID][sp++] = b; }
byte popByte(int procID) { return stack[procID][--sp]; }

float popVal(int procID, int type) {
    // int type = popByte();
    switch (type) {
        case 1:  // Char
        {
            return popChar(procID);
            break;
        }
        case 2:  // Int
        {
            return popInt(procID);
            break;
        }
        case 4:  // Float
        {
            return popFloat(procID);
            break;
        }
        default:
            break;
    }
}

// CHAR
void pushChar(int procID, char c) {
    // 1. data pushen
    // 2. type pushen
    pushByte(procID, c);
    pushByte(procID,0x01);  // push char
}
char popChar(int procID) {
    // popByte();         // pop type Char
    return popByte(procID);  // pop Char
}

// INT
void pushInt(int procID, int i) {
    // byte lb = lowByte(i);
    // byte hb = highByte(i);
    pushByte(procID, highByte(i));
    pushByte(procID, lowByte(i));
    pushByte(procID, 0x02);  // push int
}
int popInt(int procID) {
    // popByte();  // pop Int
    byte lb = popByte(procID);
    byte hb = popByte(procID);
    int i = word(hb, lb);
    return i;
}

// FLOAT
void pushFloat(int procID, float f) {
    byte *b = (byte *)&f;
    for (int i = 3; i >= 0; i--) {
        // push bytes beginnend met highbytes
        // Serial.println(b[i]);
        pushByte(procID, b[i]);
    }
    pushByte(procID, 0x04);  // push float
}
float popFloat(int procID) {
    // popByte();  // pop Float
    byte b[4];
    for (int i = 0; i < 4; i++) {
        byte temp = popByte(procID);
        // Serial.println(temp);
        b[i] = temp;  // pop bytes beginnend met lowbytes
    }

    float *f = (float *)b;

    // Serial.print("float: ");
    // Serial.println(*f);
    return *f;
}

// STRING
void pushString(int procID, char *s) {
    for (int i = 0; i < strlen(s); i++) {
        pushByte(procID, s[i]);  // push letters
    }
    pushByte(procID, 0x00);           // push terminating zero
    pushByte(procID, strlen(s) + 1);  // push length
    pushByte(procID, 0x03);           // push string
}
char *popString(int procID) {
    // popByte();               // pop type String
    int length = popByte(procID);  // pop length
    char *temp = new char[length];
    for (int i = length - 1; i >= 0; i--) {
        byte letter = popByte(procID);
        Serial.println((char)letter);
        temp[i] = letter;  // pop de letters incl. terminating zero
    }
    return temp;
}

void debugTestStack() {
    // // Push int
    // pushInt(8);
    // // push char
    // pushChar('a');
    // // pop char en print die
    // Serial.print("\nChar: ");
    // Serial.println(popChar());
    // // pop int en print die
    // Serial.print("\nInt: ");
    // Serial.println(popInt());
    // push string
    // // char temp[5] = {'h', 'e', 'l', 'l', 'o'};
    char temp[6] = "hello";
    // pushString(temp);
    // // pop string en print die
    // Serial.print("\n String:");
    // popByte(); // pop type string
    // char *tempPop = popString();
    // Serial.println(strlen(tempPop));
    // // for (int i = 0; i < 5; i++) {
    // //     Serial.print(tempPop[i]);
    // // }
    // Serial.println(tempPop);
    // Serial.println();
    // // push float
    // pushFloat(0.2345);
    // // pop float en print die
    // Serial.print("\n FLoat:");
    // Serial.println(popFloat(), 4);
    // push int
    // pushInt(300);
    // Serial.print("\n Int:");
    // int type = popByte();
    // Serial.println(popVal(type));
    // pushChar('H');
    // Serial.print("\n Char:");
    // type = popByte();
    // Serial.println((char)popVal(type));
    int procID = 0;
    pushFloat(procID, 1.2345);
    Serial.print("\n Float:");
    int type = popByte(procID);
    Serial.println(popVal(procID, type));
}