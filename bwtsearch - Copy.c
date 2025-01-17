#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bwtfile.h"

char* CalFList (int* count, int length, char* bwt, char delimiter) {
	int* copy = malloc(sizeof(int) * NUMBER);
	memcpy(copy, count, sizeof(int) * NUMBER);

	char* fList = (char*)malloc(sizeof(char)*length);
	int i = 0;
	for(i = 0; i < length; ++i) {
		int index = CalIndex(bwt[i], delimiter);
		fList[copy[index]] = bwt[i];
		copy[index]++;
	}

	free(copy);
	return fList;
}

char* FindF(char* fPath, char* bwt, int* index, int length) {
	
}

char* ReadFList (char* fPath, char* bwt, int* index, int length) {
	FILE *fp = fopen(fPath, "r");
	if(fp == NULL) {
		//Find FList from BWT
		char* flist = FindF(fPath, bwt, index, length);
		return flist;
	}

	char* flist = (char*)malloc(sizeof(char) * length);
	memset(flist, 0, length);
	
	char* pBuffer = flist;
	while(fread(pBuffer, sizeof(char), READ_BUFFER, fp) > 0) {	
		pBuffer += READ_BUFFER;
	}
	fclose(fp);

	return flist;
}


int Occ(char* bwt, char c, int pos) {
	int o = 0;

	int i = 0;
	for(i = 0; i < pos; ++i) {
		if(bwt[i] == c)
			++o;
	}

	//printf("%c %d %d\n", c, pos, o);
	return o;
}

void Search_M(char* bwt, int* cTable, int* count, char* str, char d, int length) {
	int i = strlen(str) - 1;
	char c = str[i];
	int index  = CalIndex(c, d);
	int first = cTable[index] + 1;
	int last = first + count[index] - 1;
	//printf("i=%d, c=%c, first=%d, last=%d\n", i, c, first, last);

	while((first<=last) && (i>=1)) {
		c = str[i-1];
		index = CalIndex(c, d);
		first = cTable[index] + Occ(bwt, c, first-1) + 1;
		last = cTable[index] + Occ(bwt, c, last);
		i = i-1;
		//printf("i=%d, c=%c, first=%d, last=%d\n", i, c, first, last);
		//printf("%d\n", last-first+1);
	}

	if(last < first)
		printf("%d\n", 0);

	else
		printf("%d\n", last-first+1);
}

void Search_N(char* bwt, int* cTable, int* count, char* str, char d, int length) {
	int i = strlen(str) - 1;
	char c = str[i];
	int index  = CalIndex(c, d);
	int first = cTable[index] + 1;
	int last = first + count[index] - 1;

	while((first<=last) && (i>=1)) {
		c = str[i-1];
		index = CalIndex(c, d);
		first = cTable[index] + Occ(bwt, c, first-1) + 1;
		last = cTable[index] + Occ(bwt, c, last);
		i = i-1;

	}

	if(last < first) {
		printf("%d\n", 0);
		return;
	}

	int m = last-first+1;

	char* repeat = (char*)malloc(sizeof(char)*m);
	memset(repeat, '0', m);

	int records = 0;
	int start = first;
	int pre = start;

	while(start <= last) {
		if(repeat[start-first] == '1') {
			++start;
			pre = start;
			continue;
		}

		c = bwt[pre-1];
		if(c == d ) {
			++start;
			pre = start;
			continue;
		}

		index = CalIndex(c, d);
		pre = cTable[index] + Occ(bwt, c, pre) ;
		if(pre >= first && pre <= last) {
			records++;
			repeat[pre-first] = '1';
			if(pre < start) {
				++start;
				pre = start;
				continue;
			}
		}
	}

	printf("%d\n", m-records);

	free(repeat);

}

void Search_A(char* bwt, int* cTable, int* count, char* str, char d, int length, int* indexArr, int arrlen) {
	int i = strlen(str) - 1;
	char c = str[i];
	int index  = CalIndex(c, d);
	int first = cTable[index] + 1;
	int last = first + count[index] - 1;

	while((first<=last) && (i>=1)) {
		c = str[i-1];
		index = CalIndex(c, d);
		first = cTable[index] + Occ(bwt, c, first-1) + 1;
		last = cTable[index] + Occ(bwt, c, last);
		i = i-1;

	}

	if(last < first) {
		return;
	}

	int m = last-first+1;

	char* repeat = (char*)malloc(sizeof(char)*m);
	memset(repeat, '0', m);

	char* reclist = (char*)malloc(sizeof(char)*arrlen);
	memset(reclist, '0', arrlen);

	int start = first;
	int pre = start;

	while(start <= last) {
		if(repeat[start-first] == '1') {
			++start;
			pre = start;
			continue;
		}

		c = bwt[pre-1];
		if(c == d ) {
			//get the number of records
			int occ = Occ(bwt, c, pre);
			reclist[indexArr[occ-1] ] = '1';

			++start;
			pre = start;
			continue;
		}

		index = CalIndex(c, d);
		pre = cTable[index] + Occ(bwt, c, pre) ;
		if(pre >= first && pre <= last) {
			repeat[pre-first] = '1';
			if(pre < start) {
				++start;
				pre = start;
				continue;
			}
		}
	}

	free(repeat);

	//print records ascend
	int k = 0;
	for(k = 0; k < arrlen; ++k) {
		if(reclist[k] == '1') {
			printf("%d\n", k+1);
		}
	}

	free(reclist);
}

void Search_I(int i, int j, char* bwt, int* cTable, int* count, char d, int length) {
	/*int* noArr = (int*)malloc(sizeof(int)*arrlen);

	for(k = 0; k <= arrlen; ++k) {
		int pos = indexArr[k];
		noArr[pos-1] = k;
	}*/

	char* record = (char*)malloc(sizeof(char) * length);
	int recsize = 0;

	int k = 0;
	for(k = i; k <= j; ++k) {
		char c = bwt[k-1];
		int index = CalIndex(c, d);
		int pre = cTable[index] + Occ(bwt, c, k);
		record[recsize++] = c;
		while(c != d) {
			c = bwt[pre-1];
			if(c!=d) {
				index = CalIndex(c,d);
				pre =  cTable[index] + Occ(bwt, c, pre);
				record[recsize++] = c;
			}
		}

		while(recsize > 0) {
			printf("%c", record[recsize-1]);
			--recsize;
		}
		printf("\n");
	}
	free(record);
}

void BackwardSearch(char delimiter, char* BWTPath, char* IndexFolder, char* mode, char* pattern) {
	//printf("%c %s %s %s %s\n", delimiter, BWTPath, IndexFolder, mode, pattern);

	FILE *fp = fopen(BWTPath, "r");

	//Get File Length
	long int length = 0;

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fclose(fp);

	//printf("%d\n", length);

	//Read To Buffer
	fp = fopen(BWTPath, "r");
	char* bwt = (char*)malloc(sizeof(char) * length);
	memset(bwt, 0, length);
	
	char* pBuffer = bwt;
	while(fread(pBuffer, sizeof(char), READ_BUFFER, fp) > 0) {	
		pBuffer += READ_BUFFER;
	}
	fclose(fp);
	
	/*int i = 0;
	for(i = 0; i < length; ++i) {
		printf("%c", bwt[i]);
	}
	printf("\n");*/

	//Read Index File
	//int* indices = (int*)malloc(sizeof(int)*1);
	//*indices = 11;

	//Find F	
	//char fListPath[PATH_LENGTH] = {};	
	//TmpPath(fListPath, IndexFolder, BWTPath, ".flst");
	//printf("%s\n", fListPath);
	
	//char* fList = ReadFList(fListPath, bwtCode, indices, length);

	//Calculate cTable and F
	int* count = PreCount(bwt, delimiter, length);
	int* cTable = CTable(count); //cTable

	char* fList = CalFList (cTable, length, bwt, delimiter); //F

	//Mode
	if(strcmp(mode, "-m") == 0)
		Search_M(bwt, cTable, count, pattern, delimiter, length);		
	else if(strcmp(mode, "-n") == 0)
		Search_N(bwt, cTable, count, pattern, delimiter, length);		
	/*else if(strcmp(mode, "-a") == 0)
		Search_A(bwt, cTable, count, pattern, delimiter, length, ilist, isize);*/		
	else if(strcmp(mode, "-i") == 0) {
		char* ptr;
		int start = strtol(pattern, &ptr, 10);
		int end = strtol(ptr, NULL, 10);
		Search_I(start, end, bwt, cTable, count, delimiter, length);			
	}

	free(bwt);
	free(count);
	free(cTable);
	//free(indices);
}

int main(int argc, char *argv[]) {
	if(argc == 6) {
		char* c = argv[1];
		BackwardSearch(c[0], argv[2], argv[3], argv[4], argv[5]);	
	}


	return 0;
}
