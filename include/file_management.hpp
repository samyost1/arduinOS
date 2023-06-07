void writeFatEntry();
void storeFile(const char* filename, int fileSize /*, const char* fileData*/);
void retrieveFile(const char* filename);
void eraseFile(const char* fileName);
void freespaceEEPROM();

void debugPrintEeprom();
void debugClearEeprom();
void printFATTable();