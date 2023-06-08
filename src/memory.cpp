#include <Arduino.h>

const int MEMORYSIZE = 25;
const variable memoryTable[MEMORYSIZE];
int noOfVars = 0;

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
    }
}