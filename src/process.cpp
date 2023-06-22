#include <process.hpp>


// struct process {
//     char name[12];
//     int procID;
//     char state;  // RUNNING 'r', PAUSED 'p', TERMINATED 0
//     int pc;      // Program counter
//     int fp;      // File pointer
//     int sp;      // Stack pointer
//     int address;
// };
process processTable[PROCESS_TABLE_SIZE];
int noOfProc = 0;

int processCounter = 0;


int findPid(int id)
// Find the index of a process in the process table based on its ID
{
    for (int i = 0; i < noOfProc; i++) {
        if (processTable[i].procID == id) {
            return i;  // Return the index of the file in processTable
        }
    }
    return -1;  // File not found in processTable
}

void changeProcessState(int processIndex, char state) {
    // Change the state of a process in the process table
    if (state != 'r' && state != 'p' && state != '0') {
        Serial.println(F("Not a valid state"));
        return;
    }
    if (processTable[processIndex].state == state) {
        Serial.print(F("Process already in "));
        Serial.print(state);
        Serial.println(F(" state"));
        return;
    }
    processTable[processIndex].state = state;
    Serial.print(F("State of process '"));
    Serial.print(processTable[processIndex].procID);
    Serial.print(F("' changed to '"));
    Serial.print(state);
    Serial.println(F("'"));
}

void runProcess(const char *filename) {
    // Run a new process

    // Check if process table has space
    if (noOfProc >= PROCESS_TABLE_SIZE) {
        Serial.println(F("Error. Not enough space in the process table"));
        return;
    }
    // Check if file exists
    int fileIndex = findFileInFAT(filename);
    
    if (fileIndex == -1) {
        Serial.println(F("File does not exist."));
        return;
    }

    // Initialize a new process with default values
    process newProcess;

    // Copy the filename to the process name
    strcpy(newProcess.name, filename);
    newProcess.procID = processCounter++;
    newProcess.state = 'r';  // Init state as runnning
    newProcess.pc = 0;  // de PC is het beginadres van het bestand (uit de FAT)
    newProcess.fp = 0;
    newProcess.sp = 0;  // de SP wordt op 0 gezet
    newProcess.address = FAT[fileIndex].beginPosition;

    processTable[noOfProc++] = newProcess;

    Serial.print(F("Proces: "));
    Serial.print(newProcess.procID);
    Serial.println(F(" has been started"));
}

// Suspend a process by changing its state to paused
void suspendProcess(int id) {
    Serial.print(F("Suspending process "));
    Serial.println(id);
    int processIndex = findPid(id);
    if (processIndex == -1) {
        Serial.println(F("processId doesn't exist"));
        return;
    }

    if (processTable[processIndex].state == '0') {
        Serial.println(F("Process already ended"));
        return;
    }

    changeProcessState(processIndex, 'p');
}

// Resume a suspended process by changing its state to running
void resumeProcess(int id) {
    int processIndex = findPid(id);
    if (processIndex == -1) {
        Serial.println(F("processId doesn't exist"));
        return;
    }

    if (processTable[processIndex].state == '0') {
        Serial.println(F("Process already ended"));
        return;
    }

    changeProcessState(processIndex, 'r');
}

// Stop a process by changing its state to terminated
void stopProcess(int id) {
    int processIndex = findPid(id);
    if (processIndex == -1) {
        Serial.println(F("processId doesn't exist"));
        return;
    }

    if (processTable[processIndex].state == '0') {
        Serial.println(F("Process already ended"));
        return;
    }
    // Delete all variables of process from memory
    deleteAllVars(id);
    changeProcessState(processIndex, '0'); // Change to terminated

    // Delete process from processTable
    for (int j = 0; j < noOfProc; j++) {
        if (processTable[j].procID == id) {
            for (int i = j; i < noOfProc; i++) {
                processTable[i] = processTable[i + 1];
            }
        }
    }

    noOfProc--;
}

// Show the list of processes with their ID, state, and name
void showProcessList() {
    Serial.println(F("Process List:"));

    for (int i = 0; i < noOfProc; i++) {
        if (processTable[i].state != '0') {
            Serial.print(F("ID: "));
            Serial.print(processTable[i].procID);
            Serial.print(F(" | Status: "));
            Serial.print(processTable[i].state);
            Serial.print(F(" | Name: "));
            Serial.println(processTable[i].name);
        }
    }
}
