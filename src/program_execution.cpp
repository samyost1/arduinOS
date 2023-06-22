#include <program_execution.hpp>

void execute(int index) {
    // Execute something
    // De instructies en hun argumenten staan in een bestand in het
    // bestandssysteem, in het EEPROM. De PC wijst altijd naar de eerstvolgende
    // instructie. Die moet ingelezen worden en ge¨ınterpreteerd. Dit gaat het
    // handigst met een switch...case-structuur. Voor iedere instructie moet een
    // stukje Arduino-code geschreven worden dat eventuele argumenten inleest,
    // en de instructie uitvoert. Ook moet de PC telkens aangepast worden.
    int address = processTable[index].address;
    int procID = processTable[index].procID;
    byte currentCommand = EEPROM.read(address + processTable[index].pc);
    processTable[index].pc++;
    Serial.print("[");
    Serial.print(address + processTable[index].pc);
    Serial.print("] Current command: ");
    Serial.println(currentCommand);
    switch (currentCommand) {
        case STRING: {
            Serial.println("STRING Case");
            char string[12];
            int pointer = 0;
            do {
                string[pointer] =
                    (char)EEPROM.read(address + processTable[index].pc++);
                pointer++;
            } while (string[pointer - 1] != 0);


            for (int i = 0; i < 12; i++)
            {
                Serial.print(string[i]);
            }
            Serial.println();
            

            pushString(procID, string);  //TODO fix crashing
            break;
        }
        case STOP: {
            Serial.print("\nProcess with pid: ");
            Serial.print(procID);
            Serial.println(" is finished.");
            stopProcess(procID);
            break;
        }
        case PRINT: {
            int type = popByte(procID);
            int length = type;
            if (type == 3) {
                length = popByte(procID);
            }
            for (int i = 0; i < length; i++) {
                Serial.print(popByte(procID));
            }
            // Serial.print(popByte(procID));
            break;
        }
        // case 60 ... 127: {
        //     // Er komt een string want current command komt niet voor in
        //     // instruction_array.h
        //     Serial.println("hier");
        //     // pushByte(processTable[index].procID, currentCommand);
        //     break;
        // }
        default: {
            // pushByte(processTable[index].procID, currentCommand);
            break;
        }
    }
}

void runProcesses() {
    // Serial.println("hier1");
    for (int i = 0; i < noOfProc; i++) {
        // Serial.println("hier2");

        if (processTable[i].state == 'r') {
            execute(i);
        }
    }
}
