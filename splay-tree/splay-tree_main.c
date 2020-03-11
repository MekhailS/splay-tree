#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include "splay-tree.h"

#pragma warning(disable:4996)


int main()
{
	/*
	SPL_TREE tree;
	InitTree(&tree);
	HASH_TABLE hashTable;
	DATA data = { 0 };
	int read_flag = 1;
	char word[258];
	while (1) {
		KEY key;
		char operation;
		read_flag = scanf("%c ", &operation);
		if (!((read_flag) && (read_flag != EOF)))
			break;
		if (isspace((int)operation))
			continue;
		read_flag = scanf("%s", word);
		key = ConvertStringToKey(word);
		if (!((read_flag) && (read_flag != EOF)))
			break;
		if (operation == 'a') {
			Insert(&tree, key, data);
		}
		else if (operation == 'r') {
			Delete(&tree, key);
			free(key.string);
		}
		else if (operation == 'f') {
			int res = Search(&tree, key);
			if (res)
				printf("yes\n");
			else
				printf("no\n");
			free(key.string);
		}
		else if (isspace((int)operation)) {
			continue;
		}
		else
			break;
	}
	*/
	driverStudy();
}
