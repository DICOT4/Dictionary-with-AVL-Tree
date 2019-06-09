#include<stdio.h>
#include<stdlib.h>

#define WORD 20
#define DESCRIPTION 500

// https://www.examword.com/day/100-new-word-definition-sentence

struct Node {
	char word[WORD];
	char description[DESCRIPTION];
	struct Node *leftChild;
	struct Node *rightChild;
	int height;
};

int height(struct Node *node) {
	if (node == NULL)
		return 0;
	return node->height;
}

int max(int a, int b) {
	return (a > b)? a : b;
}

struct Node* newNode(char word[], char description[]) {
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));

	for (int i=0; i < WORD; i++) {
        node->word[i] = word[i];
	}
	for (int i=0; i < DESCRIPTION; i++)
        node->description[i] = description[i];

	node->leftChild = NULL;
	node->rightChild = NULL;
	node->height = 1;
	return(node);
}

struct Node *rightRotate(struct Node *y) {
	struct Node *x = y->leftChild;
	struct Node *T2 = x->rightChild;

	x->rightChild = y;
	y->leftChild = T2;
    y->height = max(height(y->leftChild), height(y->rightChild))+1;
	x->height = max(height(x->leftChild), height(x->rightChild))+1;

	return x;
}

struct Node *leftRotate(struct Node *x) {
	struct Node *y = x->rightChild;
	struct Node *T2 = y->leftChild;

	y->leftChild = x;
	x->rightChild = T2;
    x->height = max(height(x->leftChild), height(x->rightChild))+1;
	y->height = max(height(y->leftChild), height(y->rightChild))+1;

	return y;
}

struct Node *insertHere(struct Node *root, char word[], char description[]) {
    struct Node *newN = newNode(word, description);

    newN->leftChild = root->leftChild;
    newN->rightChild = root->rightChild;
    root->height = max(height(root->leftChild), height(root->rightChild))+1;
	newN->height = max(height(newN->leftChild), height(newN->rightChild))+1;

	return newN;
}

int getBalance(struct Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->leftChild) - height(N->rightChild);
}

struct Node* insert(struct Node* node, char word[], int i, char description[]) {
    if (node == NULL)
		return(newNode(word, description));

    struct Node* newN;

    if (i == WORD-1) {
        newN = insertHere(node, word, description);
        return newN;
    }
    if (word[i] < node->word[i])
        node->leftChild = insert(node->leftChild, word, 0, description);
    else if (word[i] > node->word[i])
        node->rightChild = insert(node->rightChild, word, 0, description);
    else
        node = insert(node, word, i+1, description);

    int balance = getBalance(node);

	if (balance > 1 && word[i] < node->leftChild->word[i])
		return rightRotate(node);

	if (balance < -1 && word[i] > node->rightChild->word[i])
		return leftRotate(node);

	if (balance > 1 && word > node->leftChild->word[i]) {
		node->leftChild = leftRotate(node->leftChild);
		return rightRotate(node);
	}
	if (balance < -1 && word[i] < node->rightChild->word[i]) {
		node->rightChild = rightRotate(node->rightChild);
		return leftRotate(node);
	}

	return node;
}

void inOrder(struct Node *root) {
	if(root != NULL) {
		inOrder(root->leftChild);
		for (int i=0; i < WORD; i++) {
            printf ("%c", root->word[i]);
		}
		printf ("\t");

		inOrder(root->rightChild);
	}
}

void search(struct Node *root, char word[], int i) {
    if (root != NULL) {
        if (i == WORD-1) {
            printf ("Meaning: ");
            for (int p=0; p<DESCRIPTION; p++) {
                if (root->description[p] == NULL)
                    break;
                printf ("%c", root->description[p]);
            }
            return;
        }
        else if (word[i] < root->word[i])
            search (root->leftChild, word, 0);
        else if (word[i] > root->word[i])
            search (root->rightChild, word, 0);
        else
            search (root, word, i+1);
    }
    else
        printf ("Word not found!");
}

int main() {
    char filename[] = "dictionary.txt", word[WORD] = {}, description[DESCRIPTION] = {}, c;
    FILE *fptr;
    struct Node *root = NULL;

    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file %s", filename);
        exit(0);
    }
    c = fgetc(fptr);
    while (c != EOF) {
        for (int i=0; i < WORD; i++) {
            if (c == '&')
                break;
            word[i] = c;
            c = fgetc(fptr);
        }
        c = fgetc(fptr);
        for (int i=0; i < DESCRIPTION; i++) {
            if (c == '%' || c == EOF)
                break;
            description[i] = c;
            c = fgetc(fptr);
        }
        root = insert (root, word, 0, description);
        for (int i=0; i < WORD; i++) {
            word[i] = NULL;
        }
        for (int i=0; i < DESCRIPTION; i++) {
            description[i] = NULL;
        }
        c = fgetc(fptr);
    }
    fclose(fptr);

    printf("\n\t\t\t\t\t\tWords in Dictionary\n\n");
    inOrder(root);
    while (1) {
        printf ("\n\nEnter word to search: ");
        scanf ("%s", word);
        if (word[0] == 'e')
            exit(0);
        system("cls");

        printf("\n\t\t\t\t\t\tWords in Dictionary\n\n");
        inOrder(root);

        printf ("\n\nWord: %s\n", word);
        search (root, word, 0);
        for (int z=0; z<WORD; z++)
            word[z] = NULL;
    }
    return 0;
}
