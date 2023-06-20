#include <Arduino.h>

#include <process.hpp>
#include <stack.hpp>

struct process {
    byte name;
    int procID;
    char state;  // RUNNING 'r', PAUSED 'p', TERMINATED 0
    int pc;      // Program counter
    int fp;      // File pointer
    int sp;      // Stack pointer
    int address;
    byte stack;
};

const int PROCESS_TABLE_SIZE = 10;
int noOfProc = 0;

process processTable[PROCESS_TABLE_SIZE];

void runProcess(){
    
}