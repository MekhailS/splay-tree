#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#pragma warning(disable:4996)

typedef struct tree {
	char* key;
	int priority;
	struct tree *left, *right;
}Treap;




Treap *rightRotate(Treap *root) {
	Treap *tmp_left = root->left;
	Treap *tmp_right = tmp_left->right;
	tmp_left->right = root;
	root->left = tmp_right;

	return tmp_left;
}

Treap *leftRotate(Treap *root) {
	Treap *tmp_right = root->right;
	Treap *tmp_left = tmp_right->left;
	tmp_right->left = root;
	root->right = tmp_left;

	return tmp_right;
}

Treap* newNode(const char* key) {

	Treap* temp = (Treap*)malloc(sizeof(Treap));
	temp->key = (char*)malloc(sizeof(char) * strlen(key));
	strcpy(temp->key, key);
	temp->priority = rand();
	temp->left = temp->right = NULL;
	return temp;
}

Treap* Search(Treap* root, char* key) {
	if (root == NULL)
	{
		printf("no\n");
		return root;
	}

	Treap *cur = root;

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


Treap* Adelem(Treap* root, char* key) {
	if (!root)
		return newNode(key);

	if (strcmp(root->key, key) == 0) {
		return root;
	}

	//if (key <= root->key)
	if (strcmp(root->key, key) > 0)
	{
		root->left = Adelem(root->left, key);
		if (root->left->priority > root->priority)

			root = rightRotate(root);
	}
	else
	{
		root->right = Adelem(root->right, key);
		if (root->right->priority > root->priority)
			root = leftRotate(root);
	}
	return root;
}


Treap* Remove(Treap* root, char* key) {

	if (root == NULL)
		return root;

	if (strcmp(root->key, key) > 0)
		//if (key < root->key)
		root->left = Remove(root->left, key);
	else if (strcmp(key, root->key) > 0)
		//else if (key > root->key)
		root->right = Remove(root->right, key);

	else if (root->left == NULL)
	{
		Treap *temp = root->right;
		free(root);
		root = temp;
	}

	else if (root->right == NULL)
	{
		Treap *temp = root->left;
		free(root);
		root = temp;
	}

	else if (root->left->priority < root->right->priority)
	{
		root = leftRotate(root);
		root->left = Remove(root->left, key);
	}
	else
	{
		root = rightRotate(root);
		root->right = Remove(root->right, key);
	}

	return root;
}



int main(void) {

	Treap *root = NULL;
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
			root = Adelem(root, masstr);
		}
		else if (key == 'r') {
			scanf("%s", masstr);
			root = Remove(root, masstr);
		}
		else if (key == 'f') {
			scanf("%s", masstr);
			root = Search(root, masstr);
		}
		else
			return 0;
	}

	return 0;
}