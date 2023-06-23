#include <file_management.hpp>

// // Structure for a FAT entry
// struct FATEntry {
//     char name[12];
//     int beginPosition;
//     int length;
// };

// const int MAX_FILES = 10;
int noOfFiles;

FATEntry FAT[MAX_FILES];

void writeFATEntry(int index, const FATEntry& entry) {
    int address = sizeof(noOfFiles) + (index * sizeof(FATEntry));
    EEPROM.put(address, entry);
}

FATEntry readFATEntry(int index) {
    FATEntry entry;
    int address = sizeof(noOfFiles) + (index * sizeof(FATEntry));
    EEPROM.get(address, entry);
    return entry;
}

void writeFAT() {
    int address = 0;
    EEPROM.put(address, noOfFiles);
    address += sizeof(noOfFiles);
    for (int i = 0; i < noOfFiles; i++) {
        EEPROM.put(address, FAT[i]);
        address += sizeof(FATEntry);
    }
}

void readFAT() {
    int address = 0;
    EEPROM.get(address, noOfFiles);
    address += sizeof(noOfFiles);
    for (int i = 0; i < MAX_FILES; i++) {
        FAT[i] = readFATEntry(i);
    }
}

void sort() {
    // Sort the FAT entries by position using bubble sort
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < noOfFiles - 1; i++) {
            if (FAT[i].beginPosition > FAT[i + 1].beginPosition) {
                FATEntry temp = FAT[i];
                FAT[i] = FAT[i + 1];
                FAT[i + 1] = temp;
                sorted = false;
            }
        }
    }
}

int findAvailablePosition(int fileSize) {
    sort();

    int systemMemory = (sizeof(FATEntry) * MAX_FILES) + 1;

    // Check if there is enough space before first FAT entry
    if (FAT[0].beginPosition - systemMemory >= fileSize) {
        return systemMemory;
    }

    // Check for available space between FAT entries
    for (int i = 0; i < noOfFiles - 1; i++) {
        int availableSpace =
            FAT[i + 1].beginPosition - (FAT[i].beginPosition + FAT[i].length);
        if (availableSpace >= fileSize) {
            return FAT[i].beginPosition + FAT[i].length;
        }
    }

    // Check if there is enough space after last FAT entry
    int lastFileEnd =
        FAT[noOfFiles - 1].beginPosition + FAT[noOfFiles - 1].length;
    if (EEPROM.length() - lastFileEnd >= fileSize) {
        if (noOfFiles == 0) {
            return lastFileEnd + systemMemory;
        }
        return lastFileEnd;
    }

    // No available space found
    return -1;
}

int findFileInFAT(const char* fileName) {
    readFAT();  // Load FAT from EEPROM
    for (int i = 0; i < noOfFiles; i++) {
        if (strcmp(FAT[i].name, fileName) == 0) {
            return i;  // Return the index of the file in FAT
        }
    }
    return -1;  // File not found in FAT
}

void storeFile(const char* filename, int fileSize /*, const char* fileData*/) {
    Serial.println(F("Input File Data:"));
    Serial.print(F(">"));
    char fileData[fileSize];
    while (Serial.available() == 0) {
        // Wait till end of input
    }
    // char fileData[Serial.available()]; if not longer than fileSize

    for (int i = 0; i < fileSize; i++) {
        if (Serial.available() != 0) {
            fileData[i] = Serial.read();
        } else {
            fileData[i] = 32;
        }
        delayMicroseconds(1042);
    }

    // Clear Serial buffer
    while (Serial.available()) {
        Serial.read();
        delayMicroseconds(1042);
    }

    Serial.print(F("WriteFile for File: '"));
    Serial.print(filename);
    Serial.print(F("', size: '"));
    Serial.print(fileSize);
    Serial.println(F("'."));

    readFAT();  // Ensure FAT is up to date with EEPROM

    if (noOfFiles >= MAX_FILES) {
        Serial.println(
            F("Maximum number of files reached. Cannot store the file."));
        return;
    }
    if (findFileInFAT(filename) != -1) {
        Serial.println(F(
            "File with the same name already exists. Please choose a different "
            "name."));
        return;
    }

    // Find an available position to store the file
    int position = findAvailablePosition(fileSize);
    if (position == -1) {
        Serial.println(F("Error: No space left for file."));
        return;
    }
    // --------------------------

    // fill the file info in a FAT entry
    FATEntry file = {};
    strcpy(file.name, filename);
    file.beginPosition = position;
    file.length = fileSize;

    // write the FAT entry to the EEPROM
    FAT[noOfFiles] = file;
    // int fatAddress = sizeof(FATEntry) * noOfFiles + 1;
    // EEPROM.put(fatAddress, file);
    noOfFiles++;
    sort();
    writeFAT();
    // write data to the EEPROM
    fileSize++;
    for (int i = 0; i < fileSize; i++) {
        EEPROM.write(position, fileData[i]);
        position++;
    }

    Serial.println(F("Storing done."));
}

// Function to retrieve and print a file from the file system
void retrieveFile(const char* filename) {
    readFAT();  // Ensure FAT is up to date

    // Check if file exists
    int fatIndex = findFileInFAT(filename);
    if (fatIndex == -1) {
        Serial.println(F("File not found."));
        return;
    }

    int fileIndex = FAT[fatIndex].beginPosition;

    Serial.print(F("\nContent: "));
    for (int i = 0; i < FAT[fatIndex].length; i++) {
        Serial.print((char)EEPROM.read(fileIndex));
        fileIndex++;
    }
    Serial.print(F("\n"));
    Serial.println(F("End of File Content."));
}

void eraseFile(const char* fileName) {
    // Check if file exists
    readFAT();
    int fatIndex = findFileInFAT(fileName);
    if (fatIndex == -1) {
        Serial.println(F("File not found."));
        return;
    }

    for (int i = fatIndex; i < noOfFiles; i++) {
        FAT[i] = FAT[i + 1];
    }
    FATEntry emptyEntry;
    FAT[noOfFiles - 1] = emptyEntry;
    noOfFiles--;

    writeFAT();
    Serial.print(F("Erased: "));
    Serial.println(fileName);
}

void freespaceEEPROM() {
    readFAT();

    int systemMemory = (sizeof(FATEntry) * MAX_FILES) + 1;

    int usedSpace = 0;
    for (int i = 0; i < noOfFiles; i++) {
        usedSpace += FAT[i].length;
    }
    int totalAvailable = EEPROM.length() - systemMemory - usedSpace;
    Serial.print(F("Available space: "));
    Serial.println(totalAvailable);
}

void printFATTable() {
    readFAT();  // Read the FAT from EEPROM
    Serial.println();
    Serial.println(F("Number of files:"));
    Serial.println(noOfFiles);
    Serial.println(F("FAT Table:"));
    Serial.println(F("----------"));

    for (int i = 0; i < noOfFiles; i++) {
        Serial.print(i);
        Serial.print(F(": Name="));
        Serial.print(FAT[i].name);
        Serial.print(F("     \tAddress = "));
        Serial.print(FAT[i].beginPosition);
        Serial.print(F("\tLength = "));
        Serial.println(FAT[i].length);

        Serial.println(F("----------"));
    }
    Serial.println(F("----------"));
    Serial.println(F("End of FAT Table"));
    Serial.println();
}

void debugClearEeprom() {
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0);
    }
    Serial.println(F("\nEEPROM CLEARED\n"));
}

void debugPrintEeprom() {
    for (int i = 0; i < EEPROM.length(); i++) {
        if (EEPROM[i] != 0) {
            Serial.print(F("-"));
            Serial.println(i);
            Serial.println(EEPROM.read(i));
        }
    }
    Serial.println(F("End of EEPROM"));
    Serial.println();
}
