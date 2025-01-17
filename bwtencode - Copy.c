#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bwtfile.h"


char* ReadText(char* input, int length) {
	FILE *fp = fopen(input, "r");
	char* readBuffer = (char*)malloc(sizeof(char) * length);
	memset(readBuffer, 0, sizeof(char)*length);
	
	char* pBuffer = readBuffer;
	while(fread(pBuffer, sizeof(char), READ_BUFFER, fp) > 0) {	
		pBuffer += READ_BUFFER;
	}
	fclose(fp);	

	return readBuffer;
}

char* PreType(char* text, char delimiter, int size) {
	char* type = (char*)malloc(sizeof(char)*size); 
	int i = size-1;
	type[i] = 'S';
	for(i = i - 1; i >= 0; --i) {
		int id1 = CalIndex(text[i], delimiter);
		int id2 = CalIndex(text[i+1], delimiter);
		if(text[i] == delimiter && text[i+1] != delimiter) 
			type[i] = 'S';
		else if(id1 > id2)
			type[i] = 'L';
		else if(id1 < id2) 
			type[i] = 'S';
		else
			type[i] = type[i+1];
	}
	/*for(i = 0; i < size; ++i) {
		printf("%c: %c\n", text[i], type[i]);
	}*/
	return type;
}

void SaveType(char* typePath, char* bucketPath, char* type, int length) {
	FILE *bfp = fopen(bucketPath, "r");
	FILE *tfp = fopen(typePath, "w");
	int i = 0;
	for(i = 0; i < length; ++i) {
		int index = 0;
		fread(&index, sizeof(int), 1, bfp);
		fwrite(type+index, sizeof(char), 1, tfp);
	}
	fclose(bfp);
	fclose(tfp);
}

void CountLS(char* type, int length, int* num, char* sl ) {
	int s = 0;
	int l = 0;

	int i = 0;
	for(i = 0; i < length-1; ++i) {
		if(type[i] == 'S') 
			++s;
		else if(type[i] == 'L') 
			++l;
	}

	if(s <= l) {
		*sl = 'S';
		*num = s+1;
	}
	else {
		*sl = 'L';
		type[length-1] = 'L';
		*num = l+1;
	}	
}

void SaveBucket(char* buck, char *text, int size, char delimiter, int* count) {
	int* copy = malloc(sizeof(int) * NUMBER);
	memcpy(copy, count, sizeof(int) * NUMBER);

	int* b = (int*)malloc(sizeof(int) * size);
	memset(b, 0, size);
	int i = 0; 
	for(i = 0; i < size; ++i) {
		int index = CalIndex(text[i], delimiter);
		b[copy[index]] = i;
		//printf("pos = %d, index = %d\n",  copy[index], i);	
		copy[index]++;
	}
	
SavePath(buck, b, size);

	free(b);
	free(copy);

//	printf("finish bucket\n");
	
/*	fp = fopen(buck, "r");
	fseek(fp, 0, SEEK_SET);
	for(i = 0; i < size; ++i) {
		int index = 0;
		fread(&index, sizeof(int), 1, fp);
		printf("%d\n", index); 
	} 
	fclose(fp);
*/
}

int* CalDistance(char* type, int size, char ls, int* max) {
	int* d = (int*)malloc(sizeof(int) * size);
	int sIndex = -1;
	int i = 0;
	for(i = 0; i < size; ++i) {
		if(sIndex == -1) {
			d[i] = 0;
		}
		else {
			d[i] = i - sIndex;
			if(d[i] > *max)
				*max = d[i];
		}

		if(type[i] == ls) {
			sIndex = i;
		}

//		printf("%d\n", d[i]);
	}
	return d;
}

void SaveDistance(char* dst, char* buck, int* dist, int length ) {
	FILE* bfp = fopen(buck, "r");
	FILE* dfp = fopen(dst, "w");

	int i = 0;
	for(i = 0; i < length; ++i) {
		int index = 0;
		fread(&index, sizeof(int), 1, bfp);
		fwrite(&dist[index], sizeof(int), 1, dfp);
	}
	
	fclose(bfp);
	fclose(dfp);
}

int* CalSList(char* buck, char* type, int size, char ls) {
	int* sList = (int*)malloc(sizeof(int)*size);
	FILE *bfp = fopen(buck, "r");
	FILE *tfp = fopen(type, "r");

	int i = 0;
	while(i < size) {
		int index = 0;
		char t = 'S';
		fread(&index, sizeof(int), 1, bfp);
		fread(&t, sizeof(char), 1, tfp);
		if(t == ls) {
			sList[i] = index;
			i++;
		}
	}
	fclose(bfp);
	fclose(tfp);

	/*for(i = 0; i < size; ++i) {
		printf("%d\n", sList[i]);
	}*/	

	return sList;
}

int* CalDistCount(char* dist, int size, int mDst) {
	//printf("md = %d\n", mDst);
	FILE *dfp = fopen(dist, "r");

	int* dCount = (int*)malloc(sizeof(int) * mDst);
	memset(dCount, 0, sizeof(int)*mDst);
	int i = 0;
	for(i = 0; i < size; ++i) {
		int dis = 0;
		fread(&dis, sizeof(int), 1, dfp);
		//printf("%d %d\n", i, dis);
		if(dis > 0) //from distance 1
			dCount[dis-1]++;
	}
	fclose(dfp);

	//SavePath(dist, dCount, mDst);
	//free(dCount);

	return dCount;
}


int** CalDLists(char* buck, char* dist, int* dCount, int length, int size) {

	int** dLists = (int**) malloc(sizeof(int*)*size);
	int i = 0;
	for(i = 0; i < size; ++i) {
		int n = dCount[i];
		dLists[i] = (int*) malloc(sizeof(int)*n);
	}
	memset(dCount, 0, sizeof(int)*size);

	FILE *bfp = fopen(buck, "r");
	FILE *dfp = fopen(dist, "r");
	
	for(i = 0; i < length; ++i) {
		int index = 0;
		int dist = 0;
		fread(&index, sizeof(int), 1, bfp);
		fread(&dist, sizeof(int), 1, dfp);
		
		//printf("%d %d\n", index, dist);
		if(dist > 0) {
			int count = dCount[dist-1];	
			dLists[dist-1][count] = index;
			dCount[dist-1]++;
		}
	}

	fclose(bfp);
	fclose(dfp);

	return dLists;
}

void SortSBuck (int* slist, char* text, int s, int e, int d, int dst) {
	if(s == e) 
		return;

	printf("s=%d, e=%d\n", s, e);

	int i = 0;
		int j = 0;
		for(j = 0; j < e; ++j) {
			slist[j] + i;
		}
	
}

void SortSList(int* slist, char* text, int size, int dst, int d) {
	int s = 0;
	int sBuck = 0;
	char f = 's';
	int i = 0;

	for(i = 0; i < size; ++i) {
		int buck = CalIndex(text[slist[i]], d);
		printf("%d %d\n", slist[i], buck);
		if(buck == 0)
			continue;
	
		if(f == 's') {
			s = i;
			sBuck = buck;
			printf("%d %d \n", s, sBuck);
			f = 'e';
		}
		else {
			if(sBuck < buck || i == size-1) {
				SortSBuck(slist, text, s, i, 1, dst);
				f == 's';
				s = i;
			}
		}
	}
	
}

void BWTEncoding(char delimiter, char* tempFolder, char* input, char* output) {
//	printf("%c %s %s %s\n", delimiter, tempFolder, input, output);
	int i = 0;

	FILE *fp = fopen(input, "r");
	if(fp == NULL) {
		return;
	}

	//Get File Length
	long int length = 0;

	fpos_t fpos;
	fgetpos(fp, &fpos);
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fsetpos(fp, &fpos);

	printf("length = %d\n", length);

	//Read To Buffer
	char* readBuffer = (char*)malloc(sizeof(char) * length);
	memset(readBuffer, 0, length);
	
	char* pBuffer = readBuffer;
	while(fread(pBuffer, sizeof(char), READ_BUFFER, fp) > 0) {	
		pBuffer += READ_BUFFER;
	}
	fclose(fp);	

	//Suffix Array
	int* count = PreCount(readBuffer, delimiter, length);
	int* ctable = CTable(count);
	free(count);

	//Bucket
	char bucketPath[PATH_LENGTH] = {};
	TmpPath(bucketPath, tempFolder, input, ".bkt");
	printf("%s\n", bucketPath);

	SaveBucket(bucketPath, readBuffer, length, delimiter, ctable);
	free(ctable);

	//Type L/S
	char* type = PreType(readBuffer, delimiter, length);		

	//Count L/S
	int lsNum = 0;
	char sl = 'S';
	CountLS(type, length, &lsNum, &sl);
	printf("%c %d\n", sl, lsNum);

	char typePath[PATH_LENGTH] = {};	
	TmpPath(typePath, tempFolder, input, ".typ");
	printf("%s\n", typePath);
	SaveType(typePath, bucketPath, type, length);
	free(type);

/*	//Distance
	int maxDst = 0;
	int* dist = CalDistance(type, length, sl, &maxDst);

	char distPath[PATH_LENGTH] = {};	
	TmpPath(distPath, tempFolder, input, ".dst");
	printf("%s\n", distPath);
	SaveDistance(distPath, bucketPath, dist, length);
	free(dist);

	//DisCount
 	//printf("d = %d, s = %d\n", maxDst, sNum);
	int* dCount = CalDistCount(distPath, length, maxDst);

	//Construct DLists ----- boundry char** ???
	int** dLists = CalDLists( bucketPath, distPath, dCount, length, maxDst);	
	free(dCount);
	remove(distPath); //Delete ".dst"
*/
	//Construct S List ----- boundry char** ???
	int* sList = CalSList(bucketPath, typePath, lsNum, sl);
	remove(typePath); //Free type
	remove(bucketPath); //Delete ".bkt"

	//Sort S List
	//SortSList(sList, readBuffer, lsNum, maxDst, delimiter);	








	free(readBuffer);

	free(sList);
/*	for(i = 0; i < maxDst; ++i) {
		free(dLists[i]);
	}
	free(dLists);
*/
	//Delete tmpFiles
//	remove(); //Delete suffix array

/*	for(i = 1; i < maxDst+1; ++i) {
		char str[32];
		sprintf(str, "%d", i);
		char dFile[PATH_LENGTH] = {};
		strcpy(dFile, dListsPath);
		strcat(dFile, str);
		remove(dFile);
	}
*/	
}

int main(int argc, char *argv[]) {
	if(argc == 5) {
		char* c = argv[1];
		BWTEncoding(c[0], argv[2], argv[3], argv[4]);	
	}

	return 0;
}
