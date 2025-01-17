#ifndef BWTFILE_H_
#define BWTFILE_H_

#define NUMBER 97
#define PATH_LENGTH 100
#define NAME_LENGTH 50

#define KB 1024
#define MB (1024*KB)

#define READ_BUFFER (25*MB)
#define INDEX_BUFFER (25*MB)

#define MAX_INT_SIZE (25*MB)
#define WRITE_BUFFER (100*KB)

int CalIndex(char c, char d);
int* PreCount(char* text, char delimiter, int size);
int* CTable(int* count);
char IndexToChar(int index, char d);

void FileName(char* name, char* path);
void TmpPath(char* tmpPath, char* tmpFolder, char* input, char* tmpTail);
void SavePath(char* file, int* data, int length);

#endif /* BWTFILE_H_ */
