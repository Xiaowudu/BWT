#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define K 1024
#define M 1024
#define S 50
#define S_1 100

void Max() {
	FILE *fp = fopen("dataFolder/max.txt", "w");
	char *str = malloc(K*M*S);
	int j = 0;
	for(;j < K*M*S-2; ++j) {
		str[j] = 'a';
	}
	str[j++] = 'b';
	str[j] = '|';

	for(j = 0; j < M; ++j) {
		fwrite(str + K*S*j, sizeof(char), K*S, fp);
	}

	fclose(fp);
	
}

void MB1() {
	FILE *fp = fopen("dataFolder/mb1.txt", "w");
	char *str = malloc(K*M*S_1);
	int j = 0;
	for(;j < K*M*S_1-1; ++j) {
		str[j] = 'a';
	}
	str[j] = '|';

	for(j = 0; j < M; ++j) {
		fwrite(str + K*S_1*j, sizeof(char), K*S_1, fp);
	}

	fclose(fp);
}

void Banana() {
	FILE *fp = fopen("dataFolder/banana.txt", "w");
	char* str = "banana|";
	fputs(str, fp);
	fclose(fp);
}

void Dummy() {
	FILE *fp = fopen("dataFolder/dummy.txt", "w");
	char* str = "Computers in industry|Data compression|Integration|Big data indexing|";
	fputs(str, fp);
	fclose(fp);
}

void Text1() {
	FILE *fp = fopen("dataFolder/text1.txt", "w");
	char* str = "abc|aaa|";
	fputs(str, fp);
	fclose(fp);
}

void KB1() {
	printf("%d kb\n", S_1);
	FILE *fp = fopen("dataFolder/kb1.txt", "w");
	char *str = malloc(K*S_1);
	int j = 0;
	for(;j < K*S_1-1; ++j) {
		str[j] = 'a';
	}
	str[j] = '|';

	for(j = 0; j < 1; ++j) {
		fwrite(str + K*S_1*j, sizeof(char), K*S_1, fp);
	}

	fclose(fp);

}
void A5() {
	FILE *fp = fopen("dataFolder/a5.txt", "w");
	char* str = "aaaaa|";
	fputs(str, fp);
	fclose(fp);
}

void MSSP() {
	FILE *fp = fopen("dataFolder/mssp.txt", "w");
	char* str = "MISSISSIPPI$";
	fputs(str, fp);
	fclose(fp);

}

void MaxIndex() {
	FILE *fp = fopen("dataFolder/maxIndex.txt", "w");
	int *index = malloc(K*M*S*sizeof(int));
	int j = 0;
	for(;j < K*M*S; ++j) {
		index[j] = 'a';
	}

	for(j = 0; j < K*S*M; ++j) {
		fwrite(index + j, sizeof(int), 1, fp);
	}

	fclose(fp);
	
}

void AAAB() {
	FILE *fp = fopen("dataFolder/aaab.txt", "w");
	char* str = "aaab|";
	fputs(str, fp);
	fclose(fp);
	
}

DummyBWT() {
	FILE *fp = fopen("XYZ/dummy.bwt", "w");
	char* str = "ngynagsan||||ttDdr  nnttrdnieB   xtsooooiiiiIcCiimmgpetrseuaanuasdper";
	fwrite(str, sizeof(char), strlen(str), fp);
	fclose(fp);

}

int main() {
	//Text1();
	//Dummy();
	Max();
	//Banana();
	//MB1();
	//KB1();
	//A5();

	//MSSP();
	//MaxIndex();
	
	//AAAB();

	DummyBWT();

	return 0;
}
