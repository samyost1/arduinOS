#include <Arduino.h>

#include <command_line_interface.hpp>
#include <file_management.hpp>
#include <memory.hpp>
#include <process.hpp>

const int BUFSIZE = 12;

static char buffer[4][BUFSIZE];
static int bufferCounter = 0;
static int argumentCounter = 0;

// Available commands
void store();
void retrieve();
void erase();
void files();
void freespace();
void run();
void list();
void suspend();
void resume();
void kill();

// void debugStack();
void debugMemory();

bool checkCommandMatch();

typedef struct {
    char name[BUFSIZE];
    void (*func)();
    int numberOfArguments;
} commandType;

static commandType command[] = {
    {"store", &store, 2}, {"retrieve", &retrieve, 1},   {"erase", &erase, 1},
    {"files", &files, 0}, {"freespace", &freespace, 0}, {"run", &run, 1},
    {"list", &list, 0},   {"suspend", &suspend, 1},     {"resume", &resume, 1},
    {"kill", &kill, 1},   {"memory", &debugMemory, 0},
};

void readCliInput() {
    if (Serial.available() > 0) {
        delayMicroseconds(1042);
        int receivedChar = Serial.read();

        if (receivedChar == 32) {
            // Space Character
            argumentCounter++;
            bufferCounter = 0;
        } else if (receivedChar == 13 || receivedChar == 10) {
            // End of input
            delayMicroseconds(1042);
            buffer[argumentCounter][bufferCounter] = '\0';
            Serial.read();
            checkCommandMatch();
            // clearBuffer
            for (int i = 0; i < 4; i++) {
                memset(buffer, 0, BUFSIZE);
            }
            bufferCounter = 0;
            argumentCounter = 0;
        } else {
            buffer[argumentCounter][bufferCounter] = receivedChar;
            bufferCounter++;
        }
    }
}

bool checkCommandMatch() {
    bool foundMatch = false;
    int commandLength = sizeof(command) / sizeof(commandType);

    for (int i = 0; i < commandLength; i++) {
        if (strcmp(command[i].name, buffer[0]) == 0) {
            if (argumentCounter != command[i].numberOfArguments) {
                Serial.print(command[i].numberOfArguments);
                Serial.println(" arguments required");
            } else {
                foundMatch = true;

                void (*func)() = command[i].func;
                func();
            }
        }
    }
    if (!foundMatch) {
        Serial.print("Command: '");
        Serial.print(buffer[0]);
        Serial.println("' not found. Available commands:");

        for (int i = 0; i < commandLength; i++) {
            Serial.print("'");
            Serial.print(command[i].name);
            Serial.print("' ");
        }
        Serial.println(".");
    }

    return foundMatch;
}

bool checkNumeric() {
    for (int i = 0; buffer[1][i] != '\0'; i++) {
        if (int(buffer[1][i]) < 48 || int(buffer[1][i]) > 57) {
            return false;
        }
    }
    return true;
}

// COMMANDS:
void store() { 
// Store a file in file system
    storeFile(buffer[1], atoi(buffer[2]) /*, buffer[3]*/); }
void retrieve() { 
// Retrieve a file from file system
    retrieveFile(buffer[1]); }
void erase() { 
// Delete a file
    eraseFile(buffer[1]); }
void files() { 
// Print a list of files
    printFATTable(); }
void freespace() { 
// Print the available space in file system
    freespaceEEPROM(); }
void run() { 
// Start a program
    runProcess(buffer[1]); }
void list() { 
// Print a list of processes
    showProcessList(); }
void suspend() {
// Suspend a process
    // int procId = (int(buffer[1][0]) > 48 || int(buffer[1][0]) < 57) ?
    if (checkNumeric()) {
        suspendProcess(atoi(buffer[1]));
    } else {
        Serial.println("Error. Invalid process ID.");
    }
}
void resume() {
// Resume a process
    if (checkNumeric()) {
        resumeProcess(atoi(buffer[1]));
    } else {
        Serial.println("Error. Invalid process ID.");
    }
}
void kill() {
// Stop a process
    if (checkNumeric()) {
        stopProcess(atoi(buffer[1]));
    } else {
        Serial.println("Error. Invalid process ID.");
    }
}
// void kill() { debugClearEeprom(); }

// -------------------------------------------------- 
// Debug functions
void debugMemory() { debugTestMemory(); }
// void debugStack() { debugTestStack(); }