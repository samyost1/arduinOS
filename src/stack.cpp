#include <stack.hpp>

// #define STACKSIZE 32

byte stack[STACKSIZE];
byte sp = 0;

void pushByte(byte b) { stack[sp++] = b; }
byte popByte() { return stack[--sp]; }


float popVal(int type) {
    // int type = popByte();
    switch (type) {
        case 1:  // Char
        {
            return popChar();
            break;
        }
        case 2:  // Int
        {
            return popInt();
            break;

        }
        case 4:  // Float
        {
            return popFloat();
            break;
        }
        default:
            break;
    }
}

// CHAR
void pushChar(char c) {
    // 1. data pushen
    // 2. type pushen
    pushByte(c);
    pushByte(0x01);  // push char
}
char popChar() {
    // popByte();         // pop type Char
    return popByte();  // pop Char
}

// INT
void pushInt(int i) {
    // byte lb = lowByte(i);
    // byte hb = highByte(i);
    pushByte(highByte(i));
    pushByte(lowByte(i));
    pushByte(0x02);  // push int
}
int popInt() {
    // popByte();  // pop Int
    byte lb = popByte();
    byte hb = popByte();
    int i = word(hb, lb);
    return i;
}

// FLOAT
void pushFloat(float f) {
    byte *b = (byte *)&f;
    for (int i = 3; i >= 0; i--) {
        // push bytes beginnend met highbytes
        // Serial.println(b[i]);
        pushByte(b[i]);
    }
    pushByte(0x04);  // push float
}
float popFloat() {
    // popByte();  // pop Float
    byte b[4];
    for (int i = 0; i < 4; i++) {
        byte temp = popByte();
        // Serial.println(temp);
        b[i] = temp;  // pop bytes beginnend met lowbytes
    }

    float *f = (float *)b;

    // Serial.print("float: ");
    // Serial.println(*f);
    return *f;
}

// STRING
void pushString(char *s) {
    for (int i = 0; i < strlen(s); i++) {
        pushByte(s[i]);  // push letters
    }
    pushByte(0x00);           // push terminating zero
    pushByte(strlen(s) + 1);  // push length
    pushByte(0x03);           // push string
}
char *popString() {
    // popByte();               // pop type String
    int length = popByte();  // pop length
    char *temp = new char[length];
    for (int i = length - 1; i >= 0; i--) {
        byte letter = popByte();
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
    pushFloat(1.2345);
    Serial.print("\n Float:");
    int type = popByte();
    Serial.println(popVal(type));
}