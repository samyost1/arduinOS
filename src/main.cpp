#include <Arduino.h>
// #include <EEPROM.h>

#include <command_line_interface.hpp>
#include <program_execution.hpp>

void writeFatEntry();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    // TODO load everything from EEPROM
    Serial.println("\nArduinOS 1.0 ready.\n");
}

void loop() {
    // put your main code here, to run repeatedly:


    readCliInput();
    // Serial.println()
    runProcesses();
}
