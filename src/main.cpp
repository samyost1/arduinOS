#include <Arduino.h>

#include <command_line_interface.hpp>
#include <program_execution.hpp>

// void writeFatEntry();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    // TODO load everything from EEPROM
    Serial.println(F("\nArduinOS 1.0 ready.\n"));
    // pinMode(13, 0);
    // delay(2000);
}

void loop() {
    // digitalWrite(13, 1);
    // delay(2000);
    // digitalWrite(13, 0);
    // put your main code here, to run repeatedly:
    readCliInput();
    // Serial.println()
    runProcesses();
}
