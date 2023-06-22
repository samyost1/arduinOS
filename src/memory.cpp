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

const int MEMORYSIZE = 10;
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
        return 0;
    }

    // Check for available space between memory entries
    for (int i = 0; i < noOfVars - 1; i++) {
        int availableSpace = memoryTable[i + 1].adress -
                             (memoryTable[i].adress + memoryTable[i].length);
        if (availableSpace >= size) {
            return memoryTable[i].adress + memoryTable[i].length;
        }
    }

    // Check if there is enough space after last memoryTable entry
    int lastEntry =
        memoryTable[noOfVars - 1].adress + memoryTable[noOfVars - 1].length;
    if (MAXRAM - (int)lastEntry >= size) {
        return lastEntry;
    }

    print1("No space found");
    return -1;
}

int findFileInMemory(byte name, int procID) {
    for (int i = 0; i < noOfVars; i++) {
        if (memoryTable[i].name == name && memoryTable[i].procID == procID) {
            return i;
        }
    }
    return -1;  // Not found
}

void addMemoryEntry(byte name, int procID, int& stackP) {
    // Check if there's space in the memory table
    if (noOfVars >= MEMORYSIZE) {
        Serial.print(F("Error. Not enough space in the memory table"));
        return;
    }

    // Check if variable is already in memorytable and should be overwritten
    int index = findFileInMemory(name, procID);

    if (index != -1) {
        // shift all variables 1 place down to delete 1 variable
        for (int i = index; i < noOfVars; i++) {
            memoryTable[i] = memoryTable[i + 1];
        }
        noOfVars--;
    }

    index = noOfVars;

    int type = popByte(procID, stackP);
    int size = (type != 3) ? type : popByte(procID, stackP);
    sortMemory();

    int newAdress = (noOfVars > 0) ? findAvailableSpace(size) : 0;
    // print2("newadress: ");
    // print1(newAdress);
    variable newVariable = {.name = name,
                            .type = type,
                            .length = size,
                            .adress = newAdress,
                            .procID = procID};

    memoryTable[index] = newVariable;

    switch (type) {
        case 1: {
            /* char */
            saveChar(popVal(procID, stackP, type), newAdress);
            break;
        }
        case 2: {
            /* int */
            saveInt(popVal(procID, stackP, type), newAdress);
            break;
        }
        case 3: {
            /* string */
            char *s = popString(procID, stackP);
            saveString(s, newAdress);
            break;
        }
        case 4: {
            /* float */
            saveFloat(popVal(procID, stackP, type), newAdress);
            break;
        }
        default:
            break;
    }

    noOfVars++;
}

void retrieveMemoryEntry(byte name, int procID, int& stackP) {
    int index = findFileInMemory(name, procID);
    if (index == -1) {
        print1("Error. This variable doesn't exist.");
        return;
    }

    int type = memoryTable[index].type;
    int length = memoryTable[index].length;
    switch (type) {
        case 1:{
            /* char */
            char temp = loadChar(memoryTable[index].adress);
            pushChar(procID, stackP, temp);
            break;
        }
        case 2:{
            /* int */
            int temp = loadInt(memoryTable[index].adress);
            pushInt(procID, stackP, temp);
            break;
        }
        case 3:{
            /* string */
            pushString(procID, stackP,
                       loadString(memoryTable[index].adress, length));
            break;
        }
        case 4:{
            /* float */
            pushFloat(procID, stackP, loadFloat(memoryTable[index].adress));
            break;
        }
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
        b[i] = RAM[adress + i];  // pop bytes beginnend met lowbytes
    }

    float *f = (float *)b;
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

void deleteAllVars(int procID) {
    // Delete all variables for a process
    for (int j = 0; j < noOfVars; j++) {
        if (memoryTable[j].procID == procID) {
            for (int i = j; i < noOfVars; i++) {
                memoryTable[i] = memoryTable[i + 1];
            }
            noOfVars--;
        }
    }
}

// --------------------------------------------------
// Debug functions
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

    Serial.println("---- Memory Table ----");
    for (int i = 0; i < noOfVars; i++) {
        Serial.print("Name: ");
        Serial.print((char)memoryTable[i].name);
        Serial.print("\tType: ");
        Serial.print(memoryTable[i].type);
        Serial.print("\tLength: ");
        Serial.print(memoryTable[i].length);
        Serial.print("\tRAM: ");
        Serial.print(RAM[memoryTable[i].adress]);
        Serial.print("\tProcID: ");
        Serial.print(memoryTable[i].procID);
        Serial.println("-----");
    }
}
