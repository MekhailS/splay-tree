#pragma once


typedef struct DATA {
	int value;
} DATA;


typedef struct KEY {
	int primaryKey;
	char* string;
}KEY;


typedef struct NODE {
	KEY key;
	DATA data;
	NODE* right_child, * left_child;
	NODE* parent;
}NODE;


typedef struct SPL_TREE {
	NODE* root;
}SPL_TREE;


void InitTree(SPL_TREE* tree);

int Search(SPL_TREE* tree, KEY key);

int Delete(SPL_TREE* tree, KEY key);

int Insert(SPL_TREE* tree, KEY key, DATA data);