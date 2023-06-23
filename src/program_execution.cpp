#include <program_execution.hpp>

typedef struct {
    int operatorName;
    float (*func)(int type, float value);
    int returnType;
} unaryFunction;

unaryFunction unary[] = {
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

typedef struct {
    int operatorName;
    float (*func)(float x, float y);
    int returnType;
} binaryFunction;

binaryFunction binary[] = {
    {PLUS, &plus, 0},
    {MINUS, &minus, 0},
    {TIMES, &times, 0},
    {DIVIDEDBY, &dividedBy, 0},
    {MODULUS, &modulus, 0},
    {EQUALS, &equals, CHAR},
    {NOTEQUALS, &notEquals, CHAR},
    {LESSTHAN, &lessThan, CHAR},
    {LESSTHANOREQUALS, &lessThanOrEquals, CHAR},
    {GREATERTHAN, &greaterThan, CHAR},
    {GREATERTHANOREQUALS, &greaterThanOrEquals, CHAR},
    {MIN, &minBetter, 0},
    {MAX, &maxBetter, 0},
    {POW, &pow, 0},
    {LOGICALAND, &logicalAnd, CHAR},
    {LOGICALOR, &logicalOR, CHAR},
    {LOGICALXOR, &logicalXOR, CHAR},
    {BITWISEAND, &bitwiseAND, 0},
    {BITWISEOR, &bitwiseOR, 0},
    {BITWISEXOR, &bitwiseXOR, 0},
};

// Function to find the index of a unary function in the unary array
int findUnaryFunc(int operatorNum) {
    for (int i = 0; i < 16; i++) {
        if (unary[i].operatorName == operatorNum) {
            return i;
        }
    }
    return -1;
}

// Function to find the index of a binary function in the binary array
int findBinaryFunc(int operatorNum) {
    for (int i = 0; i < 20; i++) {
        if (binary[i].operatorName == operatorNum) {
            return i;
        }
    }
    return -1;
}

// Function to execute a process at a given index in the processTable
void execute(int index) {
    // Execute something
    int address = processTable[index].address;
    int procID = processTable[index].procID;
    int& stackP = processTable[index].sp;
    byte currentCommand = EEPROM.read(address + processTable[index].pc);
    processTable[index].pc++;
    switch (currentCommand) {
        case CHAR: {
            // Handle CHAR command
            char temp = (char)EEPROM.read(address + processTable[index].pc++);
            pushChar(procID, stackP, temp);
            break;
        }
        case INT: {
            // Handle INT command
            int highByte = EEPROM.read(address + processTable[index].pc++);
            int lowByte = EEPROM.read(address + processTable[index].pc++);
            pushInt(procID, stackP, word(highByte, lowByte));
            break;
        }
        case STRING: {
            // Handle STRING command
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
            // Handle FLOAT command
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
            // Handle STOP command
            Serial.print(F("Process with pid: "));
            Serial.print(procID);
            Serial.println(F(" is finished."));
            deleteAllVars(procID);
            stopProcess(procID);
            Serial.println();
            break;
        }
        case 51 ... 52: {  // PRINT and PRINTLN
            // Handle PRINT and PRINTLN commands
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
            // Handle SET command
            char name = EEPROM.read(address + processTable[index].pc++);

            addMemoryEntry(name, procID, stackP);
            break;
        }
        case GET: {
            // Handle GET command
            char name = EEPROM.read(address + processTable[index].pc++);
            retrieveMemoryEntry(name, procID, stackP);
            break;
        }
        case DELAY: {
            break;
        }
        case DELAYUNTIL: {
            popByte(procID, stackP);
            int temp = popInt(procID, stackP);
            int mil = millis();
            if (temp > mil) {
                processTable[index].pc--;
                pushInt(procID, stackP, temp);
            }
            break;
        }
        case MILLIS: {
            pushInt(procID, stackP, millis());
            break;
        }
        case PINMODE: {
            popByte(procID, stackP);
            int direction = popInt(procID, stackP);
            popByte(procID, stackP);
            int pin = popInt(procID, stackP);
            pinMode(pin, direction);
            break;
        }
        case DIGITALWRITE: {
            popByte(procID, stackP);
            int status = popInt(procID, stackP);
            popByte(procID, stackP);
            int pin = popInt(procID, stackP);
            digitalWrite(pin, status);
            break;
        }
        case FORK: {
            int type = popByte(procID, stackP);
            int size = popByte(procID, stackP);
            char* fileName = popString(procID, stackP, size);
            runProcess(fileName);
            pushInt(procID,stackP,procID+1);
            break;
        }
        case WAITUNTILDONE: {
            popByte(procID,stackP);
            int runningID = popInt(procID, stackP);
            char state = processTable[runningID].state;
            if(state == 'r' || state == 'p') {
                processTable[procID].pc--;
                pushInt(procID, stackP, runningID);
            } 
            break;
        }
        case IF: {
            int jumpIfZero = EEPROM.read(address + processTable[index].pc++);
            popByte(procID, stackP);
            int condition = popInt(procID, stackP);
            if (condition != 49) {  // Not equal to ASCII '1'
                processTable[index].pc += jumpIfZero;
                pushInt(procID, stackP, jumpIfZero);
                break;
            }
            pushInt(procID, stackP, 0);
            break;
        }
        case ELSE:{
            int jumpIfNotZero = EEPROM.read(address + processTable[index].pc++);
            popByte(procID, stackP);
            int jumpIfZero = popInt(procID, stackP);
            // Serial.print(F("JumpIfNotZero: "));;
            // Serial.println(jumpIfNotZero);
            if(jumpIfZero == 0){
                processTable[index].pc += jumpIfNotZero;
            }
            pushInt(procID, stackP, 0);
            break;
        }
        case ENDIF: {
            popByte(procID, stackP);
            popInt(procID, stackP);
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
            // Handle unary functions
            int type = popByte(procID, stackP);
            float value = popVal(procID, stackP, type);

            float newValue =
                unary[findUnaryFunc(currentCommand)].func(type, value);

            int returnType =
                (unary[findUnaryFunc(currentCommand)].returnType == 0)
                    ? type
                    : unary[findUnaryFunc(currentCommand)].returnType;
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
                    pushFloat(procID, stackP, (float)newValue);
                    break;
                }
                default:
                    Serial.println(F("Execute: Default case"));
                    break;
            }
            break;
        }
        case 9 ... 13:
        case 15 ... 23:
        case 25 ... 27:
        case 35 ... 36:
        case 40: {
            // Handle binary functions
            int typeY = popByte(procID, stackP);
            float y = popVal(procID, stackP, typeY);
            int typeX = popByte(procID, stackP);
            float x = popVal(procID, stackP, typeY);

            float newValue = binary[findBinaryFunc(currentCommand)].func(x, y);
            int returnType =
                (binary[findBinaryFunc(currentCommand)].returnType == 0)
                    ? max(typeY, typeX)
                    : binary[findBinaryFunc(currentCommand)].returnType;

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
            Serial.println(F("Error. Unkown command."));
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
