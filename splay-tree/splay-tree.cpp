#include "splay-tree.h"
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>


typedef enum SPL_OPERATION {
	NONE, 
	ZIG,
	ZIG_ZIG,
	ZIG_ZAG
}SPL_OPERATION;


NODE* GetMaxNode(SPL_TREE* tree);
void RotateLeft(SPL_TREE* tree, NODE* upperNode);
void RotateRight(SPL_TREE* tree, NODE* upperNode);

int CompareString(char* s1, char* s2);
int MySign(int x);


int CompareKey(KEY a, KEY b) {
	if (a.primaryKey == b.primaryKey) {
		return(CompareString(a.string, b.string));
	}
	return MySign(a.primaryKey - b.primaryKey);
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


SPL_OPERATION DetectOperationCase(SPL_TREE* tree, NODE* targetNode) {
	if (targetNode == tree->root)
		return NONE;
	if (targetNode->parent == tree->root)
		return ZIG;
	NODE* grandparent = targetNode->parent->parent;
	if (((grandparent->rightChild == targetNode->parent) && (targetNode->parent->rightChild == targetNode)) || \
		((grandparent->leftChild == targetNode->parent) && (targetNode->parent->leftChild == targetNode)))
		return ZIG_ZIG;
	return ZIG_ZAG;
}


void InitTree(SPL_TREE* tree) {
	tree->root = NULL;
}


void Splay(SPL_TREE* tree, NODE* targetNode) {
	if ((tree->root == NULL) || (targetNode == NULL))
		return;
	for (;;) {
		SPL_OPERATION curOperation = DetectOperationCase(tree, targetNode);
		NODE* grandparent;
		switch (curOperation) {
			case NONE:
				return;
				break;

			case ZIG:
				if (targetNode->parent->leftChild == targetNode)
					RotateRight(tree, targetNode->parent);
				else
					RotateLeft(tree, targetNode->parent);
				break;

			case ZIG_ZIG:
				grandparent = targetNode->parent->parent;
				if ((grandparent->rightChild == targetNode->parent) && (targetNode->parent->rightChild == targetNode)) {
					RotateLeft(tree, grandparent);
					RotateLeft(tree, targetNode->parent);
				}
				else {
					RotateRight(tree, grandparent);
					RotateRight(tree, targetNode->parent);
				}
				break;

			case ZIG_ZAG:
				grandparent = targetNode->parent->parent;
				if ((targetNode->parent->rightChild == targetNode) && (grandparent->leftChild == targetNode->parent)) {
					RotateLeft(tree, targetNode->parent);
					RotateRight(tree, targetNode->parent);
				}
				else {
					RotateRight(tree, targetNode->parent);
					RotateLeft(tree, targetNode->parent);
				}
				break;
		}
	}
}


int Search(SPL_TREE* tree, KEY key) {
	NODE* searchNode = tree->root;
	while (searchNode != NULL) {
		int compare = CompareKey(key, searchNode->key);
		if (compare == 0) {
			Splay(tree, searchNode);
			return 1;
		}
		if (compare > 0)//key > searchNode->key
			searchNode = searchNode->rightChild;
		else
			searchNode = searchNode->leftChild;
	}
	return 0;
}


int Delete(SPL_TREE* tree, KEY key) {
	NODE* delNode = tree->root;
	while (delNode != NULL) {
		int compare = CompareKey(key, delNode->key);
		if (compare == 0) {
			Splay(tree, delNode);
			break;
		}
		if (compare > 0) // key > delNode->key
			delNode = delNode->rightChild;
		else
			delNode = delNode->leftChild;
	}
	if (delNode == NULL)
		return 0;
	SPL_TREE leftSubtree, rightSubtree;
	leftSubtree.root = tree->root->leftChild;
	rightSubtree.root = tree->root->rightChild;
	if ((leftSubtree.root == NULL) && (rightSubtree.root == NULL)) {
		tree->root = NULL;
		//free(delNode->key.string);
		//free(delNode);
		return 1;
	}
	else if (leftSubtree.root == NULL) {
		tree->root = rightSubtree.root;
		tree->root->parent = NULL;
		//free(delNode->key.string);
		//free(delNode);
		return 1;
	}
	else if (rightSubtree.root == NULL) {
		tree->root = leftSubtree.root;
		tree->root->parent = NULL;
		//free(delNode->key.string);
		//free(delNode);
		return 1;
	}
	leftSubtree.root->parent = rightSubtree.root->parent = NULL;
	free(delNode->key.string);
	free(delNode);
	NODE* maxInLeftSubtree = GetMaxNode(&leftSubtree);
	Splay(&leftSubtree, maxInLeftSubtree);
	leftSubtree.root->rightChild = rightSubtree.root;
	tree->root = leftSubtree.root;
	tree->root->rightChild->parent = tree->root;
	return 1;
}


int Insert(SPL_TREE* tree, KEY key, DATA data) {
	NODE* toAdd = (NODE*)malloc(sizeof(NODE));
	toAdd->key = key;
	toAdd->data = data;
	toAdd->leftChild = toAdd->rightChild = toAdd->parent = NULL;
	NODE* upperNode = tree->root;
	if (upperNode == NULL) {
		tree->root = toAdd;
		return 1;
	}
	for (;;) {
		int compare = CompareKey(key, upperNode->key);
		if (compare == 0) {
			free(toAdd);
			return 0;
		}
		if (compare > 0) { //key > upperNode->key
			if (upperNode->rightChild == NULL) {
				upperNode->rightChild = toAdd;
				toAdd->parent = upperNode;
				break;
			}
			else {
				upperNode = upperNode->rightChild;
			}
		}
		else {
			if (upperNode->leftChild == NULL) {
				upperNode->leftChild = toAdd;
				toAdd->parent = upperNode;
				break;
			}
			else {
				upperNode = upperNode->leftChild;
			}
		}
	}
	Splay(tree, toAdd);
	return(1);
}


NODE* GetMaxNode(SPL_TREE* tree) {
	NODE* searchNode = tree->root;
	if (searchNode == NULL)
		return(NULL);
	while (searchNode->rightChild != NULL) {
		searchNode = searchNode->rightChild;
	}
	return(searchNode);
}


//rotation of verticle incident to upperNode and its right child
void RotateLeft(SPL_TREE *tree, NODE* upperNode) {
	NODE* grandparent = upperNode->parent;
	if (grandparent != NULL)
		if (grandparent->rightChild == upperNode)
			grandparent->rightChild = upperNode->rightChild;
		else
			grandparent->leftChild = upperNode->rightChild;
	else
		tree->root = upperNode->rightChild;
	NODE* temp = upperNode->rightChild->leftChild;
	upperNode->rightChild->leftChild = upperNode;
	upperNode->parent = upperNode->rightChild;
	upperNode->rightChild->parent = grandparent;
	upperNode->rightChild = temp;
	if (temp != NULL)
		temp->parent = upperNode;
}


void RotateRight(SPL_TREE* tree, NODE* upperNode) {
	NODE* grandparent = upperNode->parent;
	if (grandparent != NULL)
		if (grandparent->rightChild == upperNode)
			grandparent->rightChild = upperNode->leftChild;
		else
			grandparent->leftChild = upperNode->leftChild;
	else
		tree->root = upperNode->leftChild;
	NODE* temp = upperNode->leftChild->rightChild;
	upperNode->leftChild->rightChild = upperNode;
	upperNode->parent = upperNode->leftChild;
	upperNode->leftChild->parent = grandparent;
	upperNode->leftChild = temp;
	if (temp != NULL)
		temp->parent = upperNode;
}


int IsValidNode(NODE* parent, NODE* node) {
	if (node == NULL)
		return 1;
	if (parent == NULL) {
		if (node->parent != NULL)
			return 0;
	}
	if (parent != NULL) {
		if (node->parent != parent)
			return 0;
	}
	if (node->leftChild != NULL) {
		if (CompareKey(node->key, node->leftChild->key) <= 0)
			return 0;
	}
	if (node->rightChild != NULL) {
		if (CompareKey(node->key, node->rightChild->key) >= 0)
			return 0;
	}
	return (IsValidNode(node, node->leftChild) * IsValidNode(node, node->rightChild));
}


int IsValidTree(SPL_TREE* tree) {
	return (IsValidNode(NULL, tree->root));
}


int MySign(int x) {
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	return 0;
}