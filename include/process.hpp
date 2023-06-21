const int PROCESS_TABLE_SIZE = 10;

void runProcess(const char* filename);
void suspendProcess(int id);
void resumeProcess(int id);
void stopProcess(int id);
void showProcessList();