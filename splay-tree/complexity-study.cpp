#pragma warning(disable: 4996)
#include "splay-tree.h"
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define MAX_ELEM 10000
#define STRING_SIZE 1
#define TIMES_TO_STUDY 100

typedef struct STUDY_NODE {
	STUDY_NODE* next;
	STUDY_NODE* prev;
	KEY data;
}STUDY_NODE;

typedef struct STUDY_LIST {
	STUDY_NODE* head;
	int size;
}STUDY_LIST;


void InitStudyList(STUDY_LIST* list) {
	list->head = NULL;
	list->size = 0;
}

void PushToStart(STUDY_LIST* list, KEY keyToAdd) {
	STUDY_NODE* newNode = (STUDY_NODE*)malloc(sizeof(STUDY_NODE));
	list->size++;
	newNode->next = list->head;
	newNode->prev = NULL;
	newNode->data = keyToAdd;
	if (list->head != NULL)
		list->head->prev = newNode;
	list->head = newNode;
}


void DeleteNode(STUDY_LIST* list, STUDY_NODE* nodeToDel) {
	if (nodeToDel->prev != NULL){
		nodeToDel->prev->next = nodeToDel->next;
	}
	else {
		list->head = nodeToDel->next;
	}
	if (nodeToDel->next != NULL) {
		nodeToDel->next->prev = nodeToDel->prev;
	}
	free(nodeToDel);
	list->size--;
}


STUDY_NODE* GetNodeWithIndex(STUDY_LIST* list, int targetIndex) {
	if ((targetIndex >= list->size) || (list->head == NULL))
		return NULL;
	int i = 0;
	STUDY_NODE* curNode = list->head;
	while (i != targetIndex) {
		curNode = curNode->next;
		i++;
	}
	return curNode;
}


KEY generateRandomKey(int range) {
	KEY key;
	key.primaryKey = rand() % range;
	key.string = (char*)malloc(STRING_SIZE*sizeof(char)+1);
	for (int i = 0; i < STRING_SIZE; i++)
		key.string[i] = 'a'+(char)(rand() % (int('z')-int('a')));
	key.string[7] = '\0';
	return key;
}


STUDY_LIST GenerateRandomListToStudy(int size, int range) {
	STUDY_LIST list;
	InitStudyList(&list);
	while (list.size != size) {
		KEY keyToAdd = generateRandomKey(range);
		PushToStart(&list, keyToAdd);
	}
	return list;
}


void PrintStudyArrayToFile(double timeArr[], char filename[]) {
	FILE* file = fopen(filename, "w");
	for (int i = 0; i < MAX_ELEM; i++) {
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
	int range = 100000000;
	SPL_TREE tree;
	double timeInsert[MAX_ELEM] = { 0 };
	double timeSearch[MAX_ELEM] = { 0 };
	double timeDelete[MAX_ELEM] = { 0 };
	for (int k = 0; k < TIMES_TO_STUDY; k++) {
		STUDY_LIST list = GenerateRandomListToStudy(MAX_ELEM, range);
		InitTree(&tree);
		long long timeOne = 0, timeTwo = -1, freq;
		STUDY_NODE* curNode = list.head;
		for (int i = 0; i < MAX_ELEM; i++) {
			KEY keyCur = curNode->data;

			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeOne);
			Insert(&tree, keyCur, data);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeTwo);
			timeInsert[i] += (double)((timeTwo - timeOne) * 1.0 / freq * 1000000);

			int indexToSearch = rand() % (i+1);
			STUDY_NODE* nodeToFind = GetNodeWithIndex(&list, indexToSearch);
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeOne);
			Search(&tree, nodeToFind->data);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeTwo);
			timeSearch[i] += (double)((timeTwo - timeOne) * 1.0 / freq * 1000000);

			curNode = curNode->next;
		}
		for (int i = 0; i < MAX_ELEM; i++) {
			int indexToDelete = rand() % (list.size);
			STUDY_NODE* nodeToDelete = GetNodeWithIndex(&list, indexToDelete);
			QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeOne);
			Delete(&tree, nodeToDelete->data);
			QueryPerformanceCounter((LARGE_INTEGER*)&timeTwo);
			DeleteNode(&list, nodeToDelete);
			timeDelete[MAX_ELEM - 1 - i] += (double)((timeTwo - timeOne) * 1.0 / freq * 1000000);
		}
	}
	PrintStudyArrayToFile(timeInsert, (char*)"study_insert_output.txt");
	PrintStudyArrayToFile(timeSearch, (char*)"study_search_output.txt");
	PrintStudyArrayToFile(timeDelete, (char*)"study_delete_output.txt");
	return 1;
}