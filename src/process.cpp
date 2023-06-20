#include <Arduino.h>

#include <file_management.hpp>
#include <process.hpp>
#include <stack.hpp>

struct process {
    char name[12];
    int procID;
    char state;  // RUNNING 'r', PAUSED 'p', TERMINATED 0
    int pc;      // Program counter
    int fp;      // File pointer
    int sp;      // Stack pointer
    int address;
};

int noOfProc = 0;

process processTable[PROCESS_TABLE_SIZE];

int findPid(int id) {
    for (int i = 0; i < noOfProc; i++) {
        if (processTable[i].procID == id) {
            return i;  // Return the index of the file in processTable
        }
    }
    return -1;  // File not found in processTable
}

void runProcess(const char* filename) {
    // Check if process table has space
    if (noOfProc >= PROCESS_TABLE_SIZE) {
        Serial.println("Error. Not enough space in the process table");
        return;
    }
    // Check if file exists
    int fileIndex = findFileInFAT(filename);
    if (fileIndex == -1) {
        Serial.println("File does not exist.");
        return;
    }


    process newProcess = {
        .procID = ++noOfProc,
        .state = 'r',
        .pc = 0,
        .fp = 0,
        .sp = 0,
        .address = 0,
    };
    strcpy(newProcess.name, filename);
}

void suspendProcess(int id) {}

void resumeProcess(int id) {}

void stopProcess(int id) {}