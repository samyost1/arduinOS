#include <program_execution.hpp>

typedef struct {
    int operatorName;
    float (*func)(int type, float value);
    int returnType;
} operatorFunction;

operatorFunction unary[] = {
    {INCREMENT, &incrementBetter, 0},
    {DECREMENT, &decrementBetter, 0},
    {UNARYMINUS, &unaryminus, 0},
    {LOGICALNOT, &logicalNot, CHAR},
    {BITWISENOT, &bitwiseNot, 0},
    {TOCHAR, &ToChar, CHAR},
    {TOINT, &ToInt, INT},
    {TOFLOAT, &ToFloat, FLOAT},
    {ROUND, &Round, INT},
    {FLOOR, &Floor, INT},
    {CEIL, &Ceil, INT},
    {ABS, &Abs, 0},
    {SQ, &Sq, 0},
    {SQRT, &Sqrt, 0},
    {ANALOGREAD, &AnalogRead, INT},
    {DIGITALREAD, &DigitalRead, CHAR},
};

int findOperatorFunc(int operatorNum) {
    for (int i = 0; i < 16; i++) {
        if (unary[i].operatorName == operatorNum) {
            return i;
        }
    }
    return -1;
}

void execute(int index) {
    // Execute something
    int address = processTable[index].address;
    int procID = processTable[index].procID;
    int& stackP = processTable[index].sp;
    byte currentCommand = EEPROM.read(address + processTable[index].pc);
    processTable[index].pc++;
    // Serial.print(F("["));
    // Serial.print(address + processTable[index].pc);
    // Serial.print(F(", "));
    // Serial.print(processTable[index].sp);
    // Serial.print(F("] Current command: "));
    // Serial.println(currentCommand);
    switch (currentCommand) {
        case CHAR: {
            char temp = (char)EEPROM.read(address + processTable[index].pc++);
            pushChar(procID, stackP, temp);
            break;
        }
        case INT: {
            int highByte = EEPROM.read(address + processTable[index].pc++);
            int lowByte = EEPROM.read(address + processTable[index].pc++);
            pushInt(procID, stackP, word(highByte, lowByte));
            break;
        }
        case STRING: {
            // Serial.println(F("String case"));
            char string[12];
            memset(&string[0], 0, sizeof(string));  // Empty string
            int pointer = 0;
            do {
                int temp = (int)EEPROM.read(address + processTable[index].pc++);
                string[pointer] = (char)temp;
                pointer++;
            } while (string[pointer - 1] != 0);

            pushString(procID, stackP, string);
            break;
        }
        case FLOAT: {
            byte b[4];
            for (int i = 3; i >= 0; i--) {
                byte temp = EEPROM.read(address + processTable[index].pc++);
                b[i] = temp;  // pop bytes beginnend met lowbytes
            }
            float* f = (float*)b;
            // Serial.print("Float to be pushed: ");
            // Serial.println(*f);
            pushFloat(procID, stackP, *f);
            break;
        }
        case STOP: {
            Serial.print(F("Process with pid: "));
            Serial.print(procID);
            Serial.println(F(" is finished."));
            deleteAllVars(procID);
            stopProcess(procID);
            Serial.println();
            break;
        }
        case 51 ... 52: {  // PRINT and PRINTLN
            // Serial.print(F("PRINT Case with type: "));
            // Serial.println(type);
            int type = popByte(procID, stackP);
            switch (type) {
                case CHAR: {
                    Serial.print(popChar(procID, stackP));
                    break;
                }
                case INT: {
                    Serial.print(popInt(procID, stackP));
                    break;
                }
                case STRING: {
                    int size = popByte(procID, stackP);
                    Serial.print(popString(procID, stackP, size));
                    break;
                }
                case FLOAT: {
                    Serial.print(popFloat(procID, stackP), 5);
                    break;
                }
                default:
                    break;
            }
            if (currentCommand == 52) {
                Serial.println();
            }
            break;
        }
        case SET: {
            // Serial.println(F("Set case"));
            char name = EEPROM.read(address + processTable[index].pc++);

            addMemoryEntry(name, procID, stackP);
            break;
        }
        case GET: {
            // Serial.println(F("Get case"));

            char name = EEPROM.read(address + processTable[index].pc++);
            retrieveMemoryEntry(name, procID, stackP);
            break;
        }

        case 7 ... 8: 
        case 14:
        case 24:
        case 28 ... 34:
        case 37:
        case 41 ... 42:
        case 47:
        case 49: {
            int type = popByte(procID, stackP);
            float value = popVal(procID, stackP, type);

            float newValue =
                unary[findOperatorFunc(currentCommand)].func(type, value);

            int returnType =
                (unary[findOperatorFunc(currentCommand)].returnType == 0)
                    ? type
                    : unary[findOperatorFunc(currentCommand)].returnType;
            switch (returnType) {
                case CHAR: {
                    pushChar(procID, stackP, (char)newValue);
                    break;
                }
                case INT: {
                    pushInt(procID, stackP, (int)newValue);
                    break;
                }
                case FLOAT: {
                    // Serial.print("NewValue: ");
                    // Serial.println((float)newValue);
                    pushFloat(procID, stackP, (float)newValue);
                    break;
                }
                default:
                    Serial.println(F("Execute: Default case"));
                    break;
            }

            break;
        }
        default: {
            break;
        }
    }
}

void runProcesses() {
    for (int i = 0; i < noOfProc; i++) {
        if (processTable[i].state == 'r') {
            execute(i);
        }
    }
}
