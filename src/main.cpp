#include <Arduino.h>

void readCliInput();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    Serial.println("\nArduinOS 1.0 ready.\n");
}

void loop() {
    // put your main code here, to run repeatedly:

    // Serial.print("Hello World!\n");
    // char commandTest = command[1].name[12];
    // Serial.println(command[1].name[12]);
    // Serial.println(sizeof(command));

    readCliInput();
}
