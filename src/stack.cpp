#include <stack.hpp>

byte stack[PROCESS_TABLE_SIZE][STACKSIZE] = {0};
// byte sp = 0;

void pushByte(int procID, int& sp, byte b) {
    Serial.println(F("pushByte()"));
    // Serial.println(stack[procID][sp]);
    // stack[0][0] = b;
    stack[procID][sp++] = b;

    // sp++;
    // Serial.print(F("sp:"));
    // Serial.println(sp);
    // Serial.println(stack[procID][sp-1]);
}
byte popByte(int procID, int& sp) { 
    return stack[procID][--sp]; 
}

float popVal(int procID, int& sp, int type) {
    // int type = popByte();
    switch (type) {
        case 1:  // Char
        {
            return popChar(procID, sp);
            break;
        }
        case 2:  // Int
        {
            return popInt(procID, sp);
            break;
        }
        case 4:  // Float
        {
            return popFloat(procID, sp);
            break;
        }
        default:
            break;
    }
}

// CHAR
void pushChar(int procID, int& sp, char c) {
    // 1. data pushen
    // 2. type pushen
    // Serial.print(F("ProcID: "));
    // Serial.print(procID);
    // Serial.print(F("\tsp: "));
    // Serial.print(sp);
    // Serial.print(F("\tChar: "));
    // Serial.println(c);
    pushByte(procID, sp, c);
    pushByte(procID, sp, 0x01);  // push char
}
char popChar(int procID, int& sp) {
    // popByte();         // pop type Char
    return popByte(procID, sp);  // pop Char
}

// INT
void pushInt(int procID, int& sp, int i) {
    // byte lb = lowByte(i);
    // byte hb = highByte(i);
    pushByte(procID, sp, highByte(i));
    pushByte(procID, sp, lowByte(i));
    pushByte(procID, sp, 0x02);  // push int
}
int popInt(int procID, int& sp) {
    // popByte();  // pop Int
    byte lb = popByte(procID, sp);
    byte hb = popByte(procID, sp);
    int i = word(hb, lb);
    return i;
}

// FLOAT
void pushFloat(int procID, int& sp, float f) {
    byte *b = (byte *)&f;
    for (int i = 3; i >= 0; i--) {
        // push bytes beginnend met highbytes
        // Serial.println(b[i]);
        pushByte(procID, sp, b[i]);
    }
    pushByte(procID, sp, 0x04);  // push float
}
float popFloat(int procID, int& sp) {
    // popByte();  // pop Float
    byte b[4];
    for (int i = 0; i < 4; i++) {
        byte temp = popByte(procID, sp);
        // Serial.println(temp);
        b[i] = temp;  // pop bytes beginnend met lowbytes
    }

    float *f = (float *)b;

    // Serial.print(F("float: "));
    // Serial.println(*f);
    return *f;
}

// STRING
void pushString(int procID, int& sp, char *s) {
    Serial.println(F("Input print: "));
    for (int i = 0; i < 12; i++) {
        Serial.print(s[i]);
    }
    // Serial.println(F("before for loop"));
    // for (int i = 0; i < strlen(s); i++) {
    //     pushByte(procID, sp, s[i]);  // push letters
    // }
    // Serial.println(F("after for loop"));
    // pushByte(procID, sp, 0x00);           // push terminating zero
    // pushByte(procID, sp, strlen(s) + 1);  // push length
    // pushByte(procID, sp, 0x03);           // push string
}
char *popString(int procID, int& sp) {
    // popByte();               // pop type String
    int length = popByte(procID, sp);  // pop length
    char *temp = new char[length];
    for (int i = length - 1; i >= 0; i--) {
        byte letter = popByte(procID, sp);
        Serial.println((char)letter);
        temp[i] = letter;  // pop de letters incl. terminating zero
    }
    return temp;
}

// --------------------------------------------------
// Debug functions
void debugTestStack() {
    // // Push int
    // pushInt(8);
    // // push char
    // pushChar('a');
    // // pop char en print die
    // Serial.print(F("\nChar: "));
    // Serial.println(popChar());
    // // pop int en print die
    // Serial.print(F("\nInt: "));
    // Serial.println(popInt());
    // push string
    // // char temp[5] = {'h', 'e', 'l', 'l', 'o'};
    char temp[6] = "hello";
    // pushString(temp);
    // // pop string en print die
    // Serial.print(F("\n String:"));
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
    // Serial.print(F("\n FLoat:"));
    // Serial.println(popFloat(), 4);
    // push int
    // pushInt(300);
    // Serial.print(F("\n Int:"));
    // int type = popByte();
    // Serial.println(popVal(type));
    // pushChar('H');
    // Serial.print(F("\n Char:"));
    // type = popByte();
    // Serial.println((char)popVal(type));
    // int procID = 0;
    // pushFloat(procID, 1.2345);
    // Serial.print(F("\n Float:"));
    // int type = popByte(procID);
    // Serial.println(popVal(procID, type));
}