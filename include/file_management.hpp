#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include <Arduino.h>
#include <EEPROM.h>

const int MAX_FILES = 10;

// Structure for a FAT entry
struct FATEntry {
    char name[12];
    int beginPosition;
    int length;
};
extern FATEntry FAT[MAX_FILES];

void writeFatEntry();
int findFileInFAT(const char* fileName);
void storeFile(const char* filename, int fileSize /*, const char* fileData*/);
void retrieveFile(const char* filename);
void eraseFile(const char* fileName);
void freespaceEEPROM();

void debugPrintEeprom();
void debugClearEeprom();
void printFATTable();

#endif  // FILE_MANAGEMENT_H