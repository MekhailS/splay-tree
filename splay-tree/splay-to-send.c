#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


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
	struct NODE* right_child, * left_child;
	struct NODE* parent;
}NODE;


typedef struct SPL_TREE {
	struct NODE* root;
}SPL_TREE;


void InitTree(SPL_TREE* tree);

int Search(SPL_TREE* tree, KEY key);

int Delete(SPL_TREE* tree, KEY key);

int Insert(SPL_TREE* tree, KEY key, DATA data);


typedef enum SPL_OPERATION {
	NONE,
	ZIG,
	ZIG_ZIG,
	ZIG_ZAG
}SPL_OPERATION;

NODE* GetMaxNode(SPL_TREE* tree);
void RotateLeft(SPL_TREE* tree, NODE* upper_node);
void RotateRight(SPL_TREE* tree, NODE* upper_node);

int CompareString(char* s1, char* s2);
int MySign(int x);


int CompareKey(KEY a, KEY b) {
	//if (a.primaryKey == b.primaryKey) {
		//return(CompareString(a.string, b.string));
	//}
	//return MySign(a.primaryKey - b.primaryKey);
	return(strcmp(a.string, b.string));
}


int CompareString(char* s1, char* s2) {
	if (s1 == s2)
		return 0;
	int len_1 = strlen(s1);
	int len_2 = strlen(s2);
	if (len_1 == len_2) {
		for (int i = 0; i < len_1; i++) {
			if ((int)s1[i] != (int)s2[i]) {
				return MySign((int)s1[i] - (int)s2[i]);
			}
		}
	}
	else {
		return MySign(len_1 - len_2);
	}
	return 0;
}


SPL_OPERATION DetectOperationCase(SPL_TREE* tree, NODE* target_node) {
	if (target_node == tree->root)
		return NONE;
	if (target_node->parent == tree->root)
		return ZIG;
	NODE* grandparent = target_node->parent->parent;
	if (((grandparent->right_child == target_node->parent) && (target_node->parent->right_child == target_node)) || \
		((grandparent->left_child == target_node->parent) && (target_node->parent->left_child == target_node)))
		return ZIG_ZIG;
	return ZIG_ZAG;
}


void InitTree(SPL_TREE* tree) {
	tree->root = NULL;
}


void Splay(SPL_TREE* tree, NODE* target_node) {
	if ((tree->root == NULL) || (target_node == NULL))
		return;
	for (;;) {
		SPL_OPERATION cur_operation = DetectOperationCase(tree, target_node);
		NODE* grandparent;
		switch (cur_operation) {
		case NONE:
			return;
			break;

		case ZIG:
			if (target_node->parent->left_child == target_node)
				RotateRight(tree, target_node->parent);
			else
				RotateLeft(tree, target_node->parent);
			break;

		case ZIG_ZIG:
			grandparent = target_node->parent->parent;
			if ((grandparent->right_child == target_node->parent) && (target_node->parent->right_child == target_node)) {
				RotateLeft(tree, grandparent);
				RotateLeft(tree, target_node->parent);
			}
			else {
				RotateRight(tree, grandparent);
				RotateRight(tree, target_node->parent);
			}
			break;

		case ZIG_ZAG:
			grandparent = target_node->parent->parent;
			if ((target_node->parent->right_child == target_node) && (grandparent->left_child == target_node->parent)) {
				RotateLeft(tree, target_node->parent);
				RotateRight(tree, target_node->parent);
			}
			else {
				RotateRight(tree, target_node->parent);
				RotateLeft(tree, target_node->parent);
			}
			break;
		}
	}
}


int Search(SPL_TREE* tree, KEY key) {
	NODE* search_node = tree->root;
	while (search_node != NULL) {
		int compare = CompareKey(key, search_node->key);
		if (compare == 0) {
			Splay(tree, search_node);
			return 1;
		}
		if (compare > 0)//key > search_node->key
			search_node = search_node->right_child;
		else
			search_node = search_node->left_child;
	}
	return 0;
}


int Delete(SPL_TREE* tree, KEY key) {
	NODE* del_node = tree->root;
	while (del_node != NULL) {
		int compare = CompareKey(key, del_node->key);
		if (compare == 0) {
			Splay(tree, del_node);
			break;
		}
		if (compare > 0) // key > del_node->key
			del_node = del_node->right_child;
		else
			del_node = del_node->left_child;
	}
	if (del_node == NULL)
		return 0;
	SPL_TREE left_subtree, right_subtree;
	left_subtree.root = tree->root->left_child;
	right_subtree.root = tree->root->right_child;
	if ((left_subtree.root == NULL) && (right_subtree.root == NULL)) {
		tree->root = NULL;
		free(del_node->key.string);
		free(del_node);
		return 1;
	}
	if (left_subtree.root == NULL) {
		tree->root = right_subtree.root;
		right_subtree.root->parent = NULL;
		free(del_node->key.string);
		free(del_node);
		return 1;
	}
	if (right_subtree.root == NULL) {
		left_subtree.root->parent = NULL;
		tree->root = left_subtree.root;
		free(del_node->key.string);
		free(del_node);
		return 1;
	}
	left_subtree.root->parent = right_subtree.root->parent = NULL;
	free(del_node->key.string);
	free(del_node);
	NODE* max_in_left_subtree = GetMaxNode(&left_subtree);
	Splay(&left_subtree, max_in_left_subtree);
	left_subtree.root->right_child = right_subtree.root;
	tree->root = left_subtree.root;
	tree->root->right_child->parent = tree->root;
	return 1;
}


int Insert(SPL_TREE* tree, KEY key, DATA data) {
	NODE* to_add = (NODE*)malloc(sizeof(NODE));
	to_add->key = key;
	to_add->data = data;
	to_add->left_child = to_add->right_child = to_add->parent = NULL;
	NODE* upper_node = tree->root;
	if (upper_node == NULL) {
		tree->root = to_add;
		return 1;
	}
	for (;;) {
		int compare = CompareKey(key, upper_node->key);
		if (compare == 0) {
			free(to_add);
			return 0;
		}
		if (compare > 0) { //key > upper_node->key
			if (upper_node->right_child == NULL) {
				upper_node->right_child = to_add;
				to_add->parent = upper_node;
				break;
			}
			else {
				upper_node = upper_node->right_child;
			}
		}
		else {
			if (upper_node->left_child == NULL) {
				upper_node->left_child = to_add;
				to_add->parent = upper_node;
				break;
			}
			else {
				upper_node = upper_node->left_child;
			}
		}
	}
	Splay(tree, to_add);
	return(1);
}


NODE* GetMaxNode(SPL_TREE* tree) {
	NODE* search_node = tree->root;
	if (search_node == NULL)
		return(NULL);
	while (search_node->right_child != NULL) {
		search_node = search_node->right_child;
	}
	return(search_node);
}


//rotation of verticle incident to upper_node and its right child
void RotateLeft(SPL_TREE* tree, NODE* upper_node) {
	NODE* grandparent = upper_node->parent;
	if (grandparent != NULL)
		if (grandparent->right_child == upper_node)
			grandparent->right_child = upper_node->right_child;
		else
			grandparent->left_child = upper_node->right_child;
	else
		tree->root = upper_node->right_child;
	NODE* temp = upper_node->right_child->left_child;
	upper_node->right_child->left_child = upper_node;
	upper_node->parent = upper_node->right_child;
	upper_node->right_child->parent = grandparent;
	upper_node->right_child = temp;
	if (temp != NULL)
		temp->parent = upper_node;
}


void RotateRight(SPL_TREE* tree, NODE* upper_node) {
	NODE* grandparent = upper_node->parent;
	if (grandparent != NULL)
		if (grandparent->right_child == upper_node)
			grandparent->right_child = upper_node->left_child;
		else
			grandparent->left_child = upper_node->left_child;
	else
		tree->root = upper_node->left_child;
	NODE* temp = upper_node->left_child->right_child;
	upper_node->left_child->right_child = upper_node;
	upper_node->parent = upper_node->left_child;
	upper_node->left_child->parent = grandparent;
	upper_node->left_child = temp;
	if (temp != NULL)
		temp->parent = upper_node;
}


int MySign(int x) {
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	return 0;
}


KEY ConvertStringToKey(char word[]) {
	KEY key;
	key.primaryKey = 0;
	int len = strlen(word);
	key.string = (char*)malloc(sizeof(char) * len + 1);
	for (int i = 0; i < len; i++) {
		key.primaryKey += (int)word[i];
		key.string[i] = word[i];
	}
	key.string[len] = '\0';
	return(key);
}


/*int main()
{
	SPL_TREE tree;
	InitTree(&tree);
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
}*/

int main() {
	char op;
	char word[7];
	SPL_TREE tree;
	InitTree(&tree);
	DATA data = { 0 };
	while (1)
	{
		KEY key;
		do
		{
			op = getchar();
		} while (isspace(op));

		if (feof(stdin) || ferror(stdin))
			return 0;

		if (op == 'a') {
			scanf("%s", word);
			key = ConvertStringToKey(word);
			Insert(&tree, key, data);
		}
		else if (op == 'r') {
			scanf("%s", word);
			key = ConvertStringToKey(word);
			Delete(&tree, key);
		}
		else if (op == 'f') {
			scanf("%s", word);
			key = ConvertStringToKey(word);
			int res = Search(&tree, key);
			if (res)
				printf("yes\n");
			else
				printf("no\n");
		}
		else
			return 0;
	}
	return 0;
}