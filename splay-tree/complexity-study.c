#pragma warning(disable: 4996)
#include "splay-tree.h"
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define ELEM_N 10000
#define STRING_SIZE 5
#define TIMES_TO_STUDY 1000


int generateRandomInRange(int a, int b){
	int num;
	if (a == b)
		num = a;
	else
		num = rand() % (b - a) + a;
	return num;
}


KEY generateRandomKey() {
	KEY key;
	char str[STRING_SIZE];
	for (int i = 0; i < STRING_SIZE; i++)
		str[i] = 'a'+(char)(rand() % (((int)'z')-((int)'a')));
	key = ConvertStringToKey(str);
	return key;
}


KEY* generateArrayRandomKeys() {
	KEY* arr = (KEY*)malloc(sizeof(KEY) * ELEM_N);
	for (int i = 0; i < ELEM_N; i++) {
		arr[i] = generateRandomKey();
	}
	return arr;
}


void swapInArray(KEY *arr, int i, int j){
	KEY temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}


void mixElementsInArr(KEY* arr) {
	for (int i = 0; i < ELEM_N; i++) {
		swapInArray(arr, i, generateRandomInRange(i, ELEM_N - 1));
	}
}


void PrintAvgStudyArrayToFile(double timeArr[], char filename[]) {
	FILE* file = fopen(filename, "w");
	for (int i = 0; i < ELEM_N; i++) {
		timeArr[i] /= TIMES_TO_STUDY;
		fprintf(file, "%15f\n", timeArr[i]);
	}
	fclose(file);
}


//submain
int driverStudy() {
	DATA data;
	data.value = 0;
	srand(time(NULL));
	SPL_TREE tree;
	double timeInsert[ELEM_N] = { 0 };
	double timeSearch[ELEM_N] = { 0 };
	double timeDelete[ELEM_N] = { 0 };
	for (int k = 0; k < TIMES_TO_STUDY; k++) {
		KEY* arrRandom = generateArrayRandomKeys();
		InitTree(&tree);
		long long timeOne = 0, timeTwo = -1, freq;
		for (int i = 0; i < ELEM_N; i++) {
			KEY keyToAdd = arrRandom[i];
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeOne);
			Insert(&tree, keyToAdd, data);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeTwo);
			timeInsert[i] += (double)((timeTwo - timeOne) * 1.0 / freq * 1000000);

			KEY keyToSearch = arrRandom[generateRandomInRange(0, i)];
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeOne);
			Search(&tree, keyToSearch);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeTwo);
			timeSearch[i] += (double)((timeTwo - timeOne) * 1.0 / freq * 1000000);
		}
		mixElementsInArr(arrRandom);
		for (int i = 0; i < ELEM_N; i++) {
			KEY keyToDelete = arrRandom[i];
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeOne);
			Delete(&tree, keyToDelete);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeTwo);
			timeDelete[ELEM_N - 1 - i] += (double)((timeTwo - timeOne) * 1.0 / freq * 1000000);
		}
	}
	PrintAvgStudyArrayToFile(timeInsert, (char*)"study_insert_output.txt");
	PrintAvgStudyArrayToFile(timeSearch, (char*)"study_search_output.txt");
	PrintAvgStudyArrayToFile(timeDelete, (char*)"study_delete_output.txt");
	return 1;
}