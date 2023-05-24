#include <Arduino.h>
#include <EEPROM.h>

// Structure for a FAT entry
struct FATEntry {
    char name[12];
    int beginPosition;
    int length;
};

const int MAX_FILES = 10;
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
        Serial.println("TEST_1");
        return systemMemory;
    }

    // Check for available space between FAT entries
    for (int i = 0; i < noOfFiles - 1; i++) {
        int availableSpace =
            FAT[i + 1].beginPosition - (FAT[i].beginPosition + FAT[i].length);
        if (availableSpace >= fileSize) {
            Serial.println("TEST_2");
            return FAT[i].beginPosition + FAT[i].length;
        }
    }

    Serial.println("noOfFiles:");
    Serial.println(noOfFiles);
    // Check if there is enough space after last FAT entry
    int lastFileEnd =
        FAT[noOfFiles - 1].beginPosition + FAT[noOfFiles - 1].length;
    if (EEPROM.length() - lastFileEnd >= fileSize) {
        Serial.println("TEST_3");
        if (noOfFiles == 0) {
            return lastFileEnd + systemMemory;
        }
        return lastFileEnd;
    }

    // No available space found
    Serial.println("TEST_4");
    return -1;
}

int findFileInFAT(const char* fileName) {
    for (int i = 0; i < noOfFiles; i++) {
        if (strcmp(FAT[i].name, fileName) == 0) {
            // return FAT[i].beginPosition;
            return i;  // Return the index of the file in FAT
        }
    }
    return -1;  // File not found in FAT
}

void storeFile(const char* filename, int fileSize, const char* fileData) {
    Serial.print("WriteFile for File: '");
    Serial.print(filename);
    Serial.print("', size: '");
    Serial.print(fileSize);
    Serial.print("', data: '");
    Serial.print(fileData);
    Serial.println("'.");

    readFAT();  // Ensure FAT is up to date with EEPROM

    if (noOfFiles >= MAX_FILES) {
        Serial.println(
            "Maximum number of files reached. Cannot store the file.");
        return;
    }

    if (findFileInFAT(filename) != -1) {
        Serial.println(
            "File with the same name already exists. Please choose a different "
            "name.");
        return;
    }

    // Find an available position to store the file
    int position = findAvailablePosition(fileSize);
    if (position == -1) {
        Serial.println("Error: No space left for file.");
        return;
    }

    Serial.println("Postion:");
    Serial.println(position);
    Serial.println();

    // --------------------------

    // fill the file info in a FAT entry
    FATEntry file = {};
    strcpy(file.name, filename);
    file.beginPosition = position;
    file.length = fileSize;

    // write the FAT entry to the EEPROM
    FAT[noOfFiles] = file;
    int fatAddress = sizeof(FATEntry) * noOfFiles + 1;
    Serial.print("fatAddress:");
    Serial.println(fatAddress);
    EEPROM.put(fatAddress, file);
    noOfFiles++;
    writeFAT();

    // write data to the EEPROM
    fileSize++;
    for (int i = 0; i < fileSize; i++) {
        EEPROM.write(position, fileData[i]);
        position++;
    }
    // --------------------------

    Serial.print("Filename:\t");
    Serial.println(filename);
    Serial.print("Size:\t\t");
    Serial.println(fileSize);
    Serial.println("Storing done.");
}

// Function to retrieve and print a file from the file system
void retrieveFile(const char* filename) {
    readFAT();  // Ensure FAT is up to date

    // Check if file exists
    int fatIndex = findFileInFAT(filename);
    if (fatIndex == -1) {
        Serial.println("File not found.");
        return;
    }

    int fileIndex = FAT[fatIndex].beginPosition;

    Serial.print("\nContent: ");
    for (int i = 0; i < FAT[fatIndex].length; i++) {
        Serial.print((char)EEPROM.read(fileIndex));
        fileIndex++;
    }
    Serial.print("\n");
    Serial.println("End of File Content.");
}

void debugClearEeprom() {
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0);
    }
    Serial.println("\nEEPROM CLEARED\n");
}

void printFATTable() {
    readFAT();  // Read the FAT from EEPROM
    Serial.println();
    Serial.println("Number of files:");
    Serial.println(noOfFiles);
    Serial.println("FAT Table:");
    Serial.println("----------");

    for (int i = 0; i < noOfFiles; i++) {
        Serial.print("File ");
        Serial.print(i);
        Serial.print(": Name=");
        Serial.print(FAT[i].name);
        Serial.print(", Begin Position=");
        Serial.print(FAT[i].beginPosition);
        Serial.print(", Length=");
        Serial.println(FAT[i].length);

        Serial.println("File Data:");
        for (int j = 0; j < FAT[i].length; j++) {
            char data = EEPROM.read(FAT[i].beginPosition + j);
            Serial.print(data);
        }
        Serial.println();
        Serial.println("----------");
    }
    Serial.println("----------");
    Serial.println("End of FAT Table");
    Serial.println();
}

void debugPrintEeprom() {
    for (int i = 0; i < EEPROM.length(); i++) {
        if (EEPROM[i] != 0) {
            Serial.print('-');
            Serial.println(i);
            Serial.println(EEPROM.read(i));
            // Serial.println();
        }
    }
    Serial.println("End of EEPROM");
    Serial.println();
}
