#include <Arduino.h>

#include <memory.hpp>

#define print2(w) Serial.print(w);
#define print1(w) Serial.println(w);

struct variable {
    byte name;
    /*
        CHAR = 1
        INT = 2
        STRING= 3
        FLOAT = 4
    */
    int type;
    int length;
    int adress;
    int procID;
};

const int MEMORYSIZE = 1;
const int MAXRAM = sizeof(variable) * MEMORYSIZE;
int noOfVars = 0;

variable memoryTable[MEMORYSIZE];
byte RAM[MAXRAM];

void sortMemory() {
    // Sort the memoryTable entries by position using bubble sort
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < noOfVars - 1; i++) {
            if (memoryTable[i].adress > memoryTable[i + 1].adress) {
                variable temp = memoryTable[i];
                memoryTable[i] = memoryTable[i + 1];
                memoryTable[i + 1] = temp;
                sorted = false;
            }
        }
    }
}

int findAvailableSpace(int size) {
    if (memoryTable[0].adress >= size) {
        Serial.println("Space found before first entry");
        return 0;
    }

    // Check for available space between memory entries
    for (int i = 0; i < noOfVars - 1; i++) {
        int availableSpace = memoryTable[i + 1].adress -
                             (memoryTable[i].adress + memoryTable[i].length);
        if (availableSpace >= size) {
            Serial.println("Enough space found");
            return memoryTable[i].adress + memoryTable[i].length;
        }
    }

    // Check if there is enough space after last memoryTable entry
    int lastEntry =
        memoryTable[noOfVars - 1].adress + memoryTable[noOfVars - 1].length;
    if (MAXRAM - (int)lastEntry >= size) {
        Serial.println("Space found after last entry");
        return lastEntry;
    }

    print1("No space found");
    return -1;
}

int findFileInMemory(byte name, int id) {
    for (int i = 0; i < noOfVars; i++) {
        if (memoryTable[i].name == name && memoryTable[i].procID == id) {
            return i;
        }
    }
    return -1;  // Not found
}

void addMemoryEntry(byte name, int id) {
    // Check if there's space in the memory table
    if (noOfVars >= MEMORYSIZE) {
        Serial.print("Error. Not enough space in the memory table");
        return;
    }

    // Check if variable is already in memorytable and should be overwritten
    int index = findFileInMemory(name, id);

    if (index != -1) {
        // shift all variables 1 place down to delete 1 variable
        for (int i = index; i < noOfVars; i++) {
            memoryTable[i] = memoryTable[i + 1];
        }
        noOfVars--;
    }

    // index is noOfVars if not found in memory
    index = (index == -1) ? noOfVars : index;

    int type = popByte(id);
    int size = (type != 3) ? type : popByte(id);
    sortMemory();

    int newAdress = (noOfVars > 0) ? findAvailableSpace(size) : 0;
    // print2("newadress: ");
    // print1(newAdress);
    variable newVariable = {.name = name,
                            .type = type,
                            .length = size,
                            .adress = newAdress,
                            .procID = id};

    memoryTable[index] = newVariable;

    // Write value to correct memoryadress in reverse order
    // print1("newadress");
    // print1(newAdress);
    // print1("size: ");
    // print1(size);
    // print1("type: ");
    // print1(type);
    switch (type) {
        case 1: {
            /* char */
            saveChar(popVal(id, type), newAdress);
            break;
        }
        case 2: {
            /* int */
            saveInt(popVal(id, type), newAdress);
            break;
        }
        case 3: {
            /* string */
            char *s = popString(id);
            saveString(s, newAdress);
            break;
        }
        case 4: {
            /* float */
            saveFloat(popVal(id, type), newAdress);
            break;
        }
        default:
            break;
    }

    noOfVars++;
}

void retrieveMemoryEntry(byte name, int id) {
    int index = findFileInMemory(name, id);
    if (index == -1) {
        print1("Error. This variable doesn't exist.");
    }
    // print2("index: ");
    // print1(index);

    int type = memoryTable[index].type;
    int length = memoryTable[index].length;
    switch (type) {
        case 1:
            /* char */
            pushChar(id, loadChar(memoryTable[index].adress));
            break;

        case 2:
            /* int */
            pushInt(id, loadInt(memoryTable[index].adress));
            break;

        case 3:
            /* string */
            pushString(id, loadString(memoryTable[index].adress, length));
            break;

        case 4:
            /* float */
            pushFloat(id, loadFloat(memoryTable[index].adress));
            break;

        default:
            break;
    }
}
void saveChar(char c, int adress) { RAM[adress] = c; }
char loadChar(int adress) { return RAM[adress]; }
void saveInt(int i, int adress) {
    RAM[adress] = highByte(i);
    RAM[adress + 1] = lowByte(i);
}
int loadInt(int adress) {
    byte hb = RAM[adress];
    byte lb = RAM[adress + 1];
    return word(hb, lb);
}
void saveFloat(float f, int adress) {
    byte *b = (byte *)&f;
    for (int i = 3; i >= 0; i--) {
        // push bytes beginnend met highbytes
        RAM[adress + i] = b[i];
    }
}
float loadFloat(int adress) {
    byte b[4];
    for (int i = 0; i < 4; i++) {
        // print1(RAM[adress + i]);
        b[i] = RAM[adress + i];  // pop bytes beginnend met lowbytes
    }

    float *f = (float *)b;
    // print1();
    // print1(*f);
    return *f;
}

void saveString(char *s, int adress) {
    for (int i = 0; i < strlen(s); i++) {
        RAM[adress + i] = s[i];  // save letters
    }
    RAM[adress + strlen(s)] = 0x00;  // save terminating zero
}
char *loadString(int adress, int length) {
    char *temp = new char[length];
    for (int i = length - 1; i >= 0; i--) {
        temp[i] = RAM[adress + i];  // pop de letters incl. terminating zero
    }
    return temp;
}

void debugTestMemory() {
    // pushInt(300);
    // addMemoryEntry('a', 12);
    // retrieveMemoryEntry('a', 12);
    // int type = popByte();
    // print2("Int: ");
    // print1(popVal(type));
    // print1();

    // pushChar('h');
    // addMemoryEntry('a', 12);
    // retrieveMemoryEntry('a', 12);
    // int type = popByte();
    // print2("Char: ");
    // print1((char)popVal(type));

    // pushFloat(1.2345);
    // addMemoryEntry('a', 12);
    // retrieveMemoryEntry('a', 12);
    // int type = popByte();
    // print2("Float: ");
    // print1(popVal(type));

    // pushString("hello");
    // addMemoryEntry('a', 12);
    // retrieveMemoryEntry('a', 12);
    // int type = popByte();
    // print2("String: ");
    // print1(popString());

    // for (int i = 0; i < noOfVars; i++)
    // {
    //     print1(memoryTable[i].name);
    //     print1(memoryTable[i].procID);
    //     print1(memoryTable[i].type);
    //     print1(memoryTable[i].length);
    //     print1(RAM[memoryTable[i].adress]);
    // }
}
