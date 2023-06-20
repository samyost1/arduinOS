#include <Arduino.h>

#include <file_management.hpp>
#include <process.hpp>
#include <stack.hpp>

struct process
{
    char name[12];
    int procID;
    char state; // RUNNING 'r', PAUSED 'p', TERMINATED 0
    int pc;     // Program counter
    int fp;     // File pointer
    int sp;     // Stack pointer
    int address;
};

int noOfProc = 0;

process processTable[PROCESS_TABLE_SIZE];

int findPid(int id)
{
    for (int i = 0; i < noOfProc; i++)
    {
        if (processTable[i].procID == id)
        {
            return i; // Return the index of the file in processTable
        }
    }
    return -1; // File not found in processTable
}

void changeProcessState(int processIndex, char state){
    if(state != 'r' || state != 'p' || state != '\0' || ){
        Serial.println("Not a valid state");
        return;
    }
    if(processTable[processIndex].state == state){
        Serial.print("Process already in ");
        Serial.print(state);
        Serial.println(" state");
        return;
    }
    processTable[processIndex].state = state;
    Serial.print("State of process '");
    Serial.print(processTable[processIndex].procID);
    Serial.print("' changed to '");
    Serial.print(state);
    Serial.println("'");
}

void runProcess(const char *filename)
{
    // Check if process table has space
    if (noOfProc >= PROCESS_TABLE_SIZE)
    {
        Serial.println("Error. Not enough space in the process table");
        return;
    }
    // Check if file exists
    int fileIndex = findFileInFAT(filename);
    if (fileIndex == -1)
    {
        Serial.println("File does not exist.");
        return;
    }

    // naam moet perse geinitialised worden met 12 characters incl \0
    process newProcess = {
        .name = "abcdefghijk", // de naam is de opgegeven naam
        .procID = ++noOfProc,  // het proces-ID komt van de teller (die met 1 opgehoogd wordt)
        .state = 'r',          // en de toestand wordt op RUNNING gezet
        .pc = 0,               // de PC is het beginadres van het bestand (uit de FAT)
        .fp = 0,
        .sp = 0, // de SP wordt op 0 gezet
        .address = 0};

    strcpy(newProcess.name, filename);

    Serial.print("Proces: ");
    Serial.print(newProcess.procID);
    Serial.println(" has been started");
}

void suspendProcess(int id)
{
    int processIndex = findPid(id);
    if (processIndex == -1)
    {
        Serial.println("processId doesn't exist");
        return;
    }

    if (processTable[processIndex].state == '\0')
    {
        Serial.println("Process already ended");
        return;
    }

    changeProcessState(processIndex, 'p');

}

void resumeProcess(int id) {
    int processIndex = findPid(id);
    if (processIndex == -1)
    {
        Serial.println("processId doesn't exist");
        return;
    }

    if (processTable[processIndex].state == '\0')
    {
        Serial.println("Process already ended");
        return;
    }

    changeProcessState(processIndex,'r');
}

void stopProcess(int id) {
    int processIndex = findPid(id);
    if (processIndex == -1)
    {
        Serial.println("processId doesn't exist");
        return;
    }

    if (processTable[processIndex].state == '\0')
    {
        Serial.println("Process already ended");
        return;
    }

    // TODO: 
    // Tevens moet je alle variabelen wissen die bij dit proces horen. Hiervoor kun je
    // de functie uit §6.10 gebruiken.


    changeProcessState(processIndex, '\0');
}