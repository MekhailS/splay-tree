#pragma once


typedef struct DATA_T {
	int value;
} DATA;


typedef struct KEY_T {
	int primaryKey;
	char* string;
}KEY;


typedef struct NODE_T {
	KEY key;
	DATA data;
	struct NODE_T* rightChild, * leftChild;
	struct NODE_T* parent;
}NODE;


typedef struct SPL_TREE_T {
	NODE* root;
}SPL_TREE;


void InitTree(SPL_TREE* tree);

int Search(SPL_TREE* tree, KEY key);

int Delete(SPL_TREE* tree, KEY key);

int Insert(SPL_TREE* tree, KEY key, DATA data);

int IsValidTree(SPL_TREE* tree);

KEY ConvertStringToKey(char word[]);

int CompareKey(KEY a, KEY b);

int driverStudy();