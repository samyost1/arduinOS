#include <Arduino.h>

const int BUFSIZE = 12;

static char buffer[4][BUFSIZE];
static int bufferCounter = 0;
static int argumentCounter = 0;

// Available commands
void store();
void retrieve();
void erase();
void files();
void list();
void suspend();
void resume();
void kill();

void readCliInput();
bool checkCommandMatch();

typedef struct {
    char name[BUFSIZE];
    void (*func)();
} commandType;

static commandType command[] = {
    {"store", &store},   {"retrieve", &retrieve}, {"erase", &erase},
    {"files", &files},   {"list", &list},         {"suspend", &suspend},
    {"resume", &resume}, {"kill", &kill},
};

void readCliInput() {
    if (Serial.available() > 0) {
        delayMicroseconds(1042);
        int receivedChar = Serial.read();

        if (receivedChar == 32) {
            argumentCounter++;
            bufferCounter = 0;
        } else if (receivedChar == 13 || receivedChar == 10) {
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

    // for (int i = 0; i < sizeof(buffer); i++) {
    //     Serial.println(buffer[i]);
    // }
    // for (int i = 0; i < sizeof(buffer); i++) {
    //     Serial.println(command[0].name[i]);
    // }
    // Serial.println("--------");
    // Serial.println(buffer[0]);
    // Serial.println(buffer[1]);
    // Serial.println(buffer[2]);
    // Serial.println(buffer[3]);
    // Serial.println("--------");

    for (int i = 0; i < commandLength; i++) {
        if (strcmp(command[i].name, buffer[0]) == 0) {
            // Serial.println("MATCH FOUND with: ");
            // Serial.println(command[i].name);
            foundMatch = true;

            void (*func)() = command[i].func;
            func();
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

// COMMANDS:
void store() { Serial.println("eyo"); }
void retrieve() {}
void erase() {}
void files() {}
void list() {}
void suspend() {}
void resume() {}
void kill() {}