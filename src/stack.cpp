#include <stack.hpp>

#define STACKSIZE 32

byte stack[STACKSIZE];
byte sp = 0;

// TODO: nu heb ik big-endian gebruikt maar miss is arduino little-endian
void pushByte(byte b) { stack[sp++] = b; }
byte popByte() { return stack[--sp]; }

// CHAR
void pushChar(char c) {
    // 1. data pushen
    // 2. type pushen
    pushByte(c);
    pushByte(0x01);  // push char
}
char popChar() {
    popByte();         // pop type Char
    return popByte();  // pop Char
}

float popVal() {
    int type = popByte();
    switch (type) {
        case 1:  // Char
        {
            return popByte();
            break;
        }
        case 2:  // Int
        {
            // popByte();  // pop Int
            byte lb = popByte();
            byte hb = popByte();
            int i = word(hb, lb);
            return (int)i;
            break;
        }
        case 4:  // Float
        {
            // popByte(); // pop Float
            byte b[4];
            for (int i = 0; i < 4; i++) {
                b[i] = popByte();  // pop bytes beginnend met lowbytes
            }

            float *f = (float *)b;

            return *f;
            break;
        }
        default:
            break;
    }
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
    popByte();  // pop Int
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
        pushByte(b[i]);
    }
    pushByte(0x04);  // push float
}
float popFloat() {
    popByte();  // pop Float
    byte b[4];
    for (int i = 0; i < 4; i++) {
        b[i] = popByte();  // pop bytes beginnend met lowbytes
    }

    float *f = (float *)b;

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
    popByte();               // pop type String
    int length = popByte();  // pop length
    char *temp = new char[length];
    for (int i = length - 1; i >= 0; i--) {
        temp[i] = (char)popByte();  // pop de letters incl. terminating zero
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
    // // push string
    // // char temp[5] = {'h', 'e', 'l', 'l', 'o'};
    // char temp[6] = "hello";
    // pushString(temp);
    // // pop string en print die
    // Serial.print("\n String:");
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
    pushInt(300);
    // pop die int met popVal()
    Serial.print("\n Int:");
    Serial.println(popVal());
    pushChar('H');
    Serial.print("\n Char:");
    Serial.println(popVal());
    pushFloat(1.2345);
    Serial.print("\n Float:");
    Serial.println(popVal());
}