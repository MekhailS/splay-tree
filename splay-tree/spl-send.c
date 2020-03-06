#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#pragma warning(disable:4996)

typedef struct SPL {
	char* key;
	int priority;
	struct SPL *left, *right;
	struct SPL* up;
}SPL;



SPL *rightRotate(SPL *root) {
	SPL *tmp_left = root->left;
	SPL *tmp_right = tmp_left->right;
	tmp_left->right = root;
	root->left = tmp_right;

	return tmp_left;
}

SPL *leftRotate(SPL *root) {
	SPL *tmp_right = root->right;
	SPL *tmp_left = tmp_right->left;
	tmp_right->left = root;
	root->right = tmp_left;

	return tmp_right;
}


SPL* newNode(const char* key) {

	SPL* temp = (SPL*)malloc(sizeof(SPL));
	temp->key = (char*)malloc(sizeof(char) * strlen(key));
	strcpy(temp->key, key);
	temp->priority = rand();
	temp->left = temp->right = temp->up = NULL;
	return temp;
}


SPL* searchNode(SPL* root, char* key) {
	if (root == NULL)
	{
		printf("no\n");
		return root;
	}

	int flag = 0;
	if (root->up == NULL) {
		flag += 1;
	}
	if (flag < 0)
		printf("yes\n");

	SPL *cur = root;

	while (1)
	{
		if (strcmp(cur->key, key) == 0)
			//if (cur->key == value)
		{
			printf("yes\n");
			return root;
		}

		if (strcmp(cur->key, key) > 0)
			//if (cur->key > value)
		{
			if (cur->left != NULL)
				cur = cur->left;
			else
			{
				printf("no\n");
				return root;
			}
		}

		if (strcmp(key, cur->key) > 0)
			//if (cur->key < value)
		{
			if (cur->right != NULL)
				cur = cur->right;
			else
			{
				printf("no\n");
				return root;
			}
		}
	}
	return root;
}


SPL* insertNode(SPL* root, char* key) {
	if (!root)
		return newNode(key);

	if (strcmp(root->key, key) == 0) {
		return root;
	}

	//if (key <= root->key)
	if (strcmp(root->key, key) > 0)
	{
		root->left = insertNode(root->left, key);
		if (root->left->priority > root->priority)

			root = rightRotate(root);
	}
	else
	{
		root->right = insertNode(root->right, key);
		if (root->right->priority > root->priority)
			root = leftRotate(root);
	}
	return root;
}


SPL* deleteNode(SPL* root, char* key) {

	if (root == NULL)
		return root;

	if (strcmp(root->key, key) > 0)
		//if (key < root->key)
		root->left = deleteNode(root->left, key);
	else if (strcmp(key, root->key) > 0)
		//else if (key > root->key)
		root->right = deleteNode(root->right, key);

	else if (root->left == NULL)
	{
		SPL *temp = root->right;
		free(root);
		root = temp;
	}

	else if (root->right == NULL)
	{
		SPL *temp = root->left;
		free(root);
		root = temp;
	}

	else if (root->left->priority < root->right->priority)
	{
		root = leftRotate(root);
		root->left = deleteNode(root->left, key);
	}
	else
	{
		root = rightRotate(root);
		root->right = deleteNode(root->right, key);
	}

	return root;
}



int main(void) {

	SPL *root = NULL;
	char* value = NULL;
	char key;
	char masstr[7];

	while (1)
	{
		do
		{
			key = getchar();
		} while (isspace(key));

		if (feof(stdin) || ferror(stdin))
			return 0;

		if (key == 'a') {
			scanf("%s", masstr);
			root = insertNode(root, masstr);
		}
		else if (key == 'r') {
			scanf("%s", masstr);
			root = deleteNode(root, masstr);
		}
		else if (key == 'f') {
			scanf("%s", masstr);
			root = searchNode(root, masstr);
		}
		else
			return 0;
	}

	return 0;
}