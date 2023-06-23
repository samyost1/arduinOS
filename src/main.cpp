#include <Arduino.h>

#include <command_line_interface.hpp>
#include <program_execution.hpp>

// void writeFatEntry();

void setup() {
    Serial.begin(9600);
    Serial.println(F("\nArduinOS 1.0 ready.\n"));
}

void loop() {
    readCliInput();
    runProcesses();
}
