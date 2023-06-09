#ifndef MEMORY_H
#define MEMORY_H

#include <Arduino.h>

#include <stack.hpp>

int findFileInMemory(byte name, int id);
int findAvailableSpace(int size);

void addMemoryEntry(byte name, int procID, int& stackP);
void retrieveMemoryEntry(byte name, int procID, int& stackP);
void deleteAllVars(int id);

void saveChar(char c, int adress);
char loadChar(int adress);
void saveInt(int i, int adress);
int loadInt(int adress);
void saveFloat(float f, int adress);
float loadFloat(int adress);
void saveString(char* s, int adress);
char* loadString(int adress, int length);

void debugTestMemory();

void sortMemory();

#endif  // MEMORY_H