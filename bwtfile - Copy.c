#include "bwtfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CalIndex(char c, char d) {
	int index = 0;
		if(c == d) {
			index = 0;
		}
		else if(c == 9) {
			index = 1;
		}
		else {
			index = (int)c;
			index = index < d ? (index - 32 + 2) : (index - 32 + 1) ;
		}
	return index;
}

int* PreCount(char* text, char delimiter, int size) {
	//Count
	int* count = (int*)malloc(sizeof(int)*NUMBER);
	int i = 0;
	for(i = 0; i < NUMBER; ++i) {
		count[i] = 0;
	}

	for(i = 0; i < size; ++i) {
		int index = CalIndex(text[i], delimiter);
		count[index]++;
	}

	return count;
}

int* CTable(int* count) {
	int* cTable = (int*)malloc(sizeof(int)*NUMBER);
	int sum = 0;
	int i = 0;
	for(i = 0; i < NUMBER; ++i) {
		if(count[i] == 0)
			cTable[i] = -1;
		if(count[i] > 0) {
			int n = count[i];
			cTable[i] = sum;
			sum += n;
		}
	}

	return cTable;
}

char IndexToChar(int index, char d) {
	char c = d;
	if(index == 0)
		c = d;
	else if(index == 1)
		c = (char)9;
	else {
		c = (char)(index -2 + 32);
		c = c < d ? c : c + 1;
	}

	return c;
}

void FileName(char* name, char* path) {
	int length = strlen(path);
	int i = 0;
	for(i = length-1; i >= 0; --i) {
		if(path[i] == '/') {
			i++;
			break;
		}
	}
	strcpy(name, &path[i]);
	for(i = 0; i < length; ++i) {
		if(name[i] == '.') {
			name[i] = '\0';
			break;
		}
	}
}

void TmpPath(char* tmpPath, char* tmpFolder, char* input, char* tmpTail) {
	strcat(tmpPath, tmpFolder);
	strcat(tmpPath, "/");
	
	char name[NAME_LENGTH] = {};
	FileName(name, input);	
	
	strcat(tmpPath, name);
	strcat(tmpPath, tmpTail);
}

void SavePath(char* file, int* data, int length) {
	FILE* fp = fopen(file, "w");
	int i = 0;
	for(i = 0; i < length / MAX_INT_SIZE; ++i) {
		fwrite(data + i * MAX_INT_SIZE, sizeof(int), MAX_INT_SIZE, fp);
	}
	if(length % MAX_INT_SIZE > 0) {
		fwrite(data + i * MAX_INT_SIZE, sizeof(int), length % MAX_INT_SIZE, fp);	}
	fclose(fp);
}
