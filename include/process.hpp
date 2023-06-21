#ifndef PROCESS_H
#define PROCESS_H

#include <Arduino.h>
#include <file_management.hpp>
#include <memory.hpp>
#include <stack.hpp>

const int PROCESS_TABLE_SIZE = 10;
struct process {
    char name[12];
    int procID;
    char state;  // RUNNING 'r', PAUSED 'p', TERMINATED 0
    int pc;      // Program counter
    int fp;      // File pointer
    int sp;      // Stack pointer
    int address;
};
extern process processTable[PROCESS_TABLE_SIZE];
extern int noOfProc;

void runProcess(const char* filename);
void suspendProcess(int id);
void resumeProcess(int id);
void stopProcess(int id);
void showProcessList();

#endif  // PROCESS_H