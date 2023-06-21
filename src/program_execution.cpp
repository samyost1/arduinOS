#include <program_execution.hpp>

void execute(int index)
{
    // Execute something
    // De instructies en hun argumenten staan in een bestand in het bestandssysteem, in het EEPROM. 
    // De PC wijst altijd naar de eerstvolgende instructie. Die moet ingelezen worden en ge¨ınterpreteerd.
    // Dit gaat het handigst met een switch...case-structuur. 
    // Voor iedere instructie moet een stukje Arduino-code geschreven worden dat eventuele argumenten inleest, en de instructie uitvoert.
    // Ook moet de PC telkens aangepast worden.

    int address = processTable[index].address;
    byte currentCommand = EEPROM.read(address + processTable[index].pc);
    processTable[index].pc++;
    Serial.print("Current command: ");
    Serial.println(currentCommand);
}

void runProcesses()
{
    for (int i = 0; i < noOfProc; i++)
    {
        if (processTable[i].state == 'r')
        {
            execute(i);
        }
    }
}
