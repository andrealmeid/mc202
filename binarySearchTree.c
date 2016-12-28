/* binary search tree implementation by andre almeida */

#include <stdio.h>
#include <stdlib.h>
#define TRUE 'T'
#define FALSE 'F'

typedef struct _node{
	int data;
	struct _node *left;
	struct _node *right;
} Node;

typedef struct _root{
	Node *node;
} Root;

Node *createNode(int num);

void insert(Node **root, int num);

Node *delete(Node *root, int num, char *found);

void printNode(Node *root);

void find(Node *root, int data);

void printTree(Node *root);

void showMenu();

int main(){

	Root root;
	root.node = NULL;

	/*printf("this is a binary search tree simulator\n");
	printf("written by andre almeida.\n");
	printf("notice that the nodes can contains only integer numbers\n");
	showMenu();*/

	char cmd, run = TRUE, found = 'F';
	scanf(" %c", &cmd);
	int noods_amount, i=0, aux;

	while(run == TRUE){
		switch(cmd){
			case 'i':
				printf("node to be inserted: ");
				scanf("%d", &aux);
				insert(&root.node, aux);
				printf("node %d created\n", aux);
				break;

			case 'm':
				printf("number of nodes: ");
				scanf("%d", &noods_amount);

				if(noods_amount == 0)
					break;

				for(i=0;i<noods_amount;i++){
					printf("node #%02d: ", i+1);
					scanf("%d", &aux);
					insert(&root.node, aux);
				}
				break;

			case 'r':
				printf("node to be removed: ");
				scanf("%d", &aux);
				delete(root.node, aux, &found);
				if(found == 'F')
					printf("data not found\n");
				else
					printf("node %d deleted!\n", aux);
				break;

			case 'p':
				printf("printing tree...\n");
				if(root.node!=NULL)
					printTree(root.node);
				else
					printf("the tree is empty\n");
				printf("printing is done\n");
				break;

			case 'f':
				printf("node to be found: ");
				scanf("%d", &aux);
				find(root.node, aux);
				break;

			case 'q':
				run = FALSE;
				printf("exiting tree simulator...\n");
				return 0;
				break;

			case 'h':
				showMenu();
				break;

		}
		scanf(" %c", &cmd);
	}

	return 0;
}

Node *createNode(int num){
	Node *node = malloc(sizeof(Node));
	node->data  = num;
	node->left  = NULL;
	node->right = NULL;

	return node;
}

void insert(Node **root, int num){
	if((*root) == NULL)
		(*root) = createNode(num);

	else if((*root)->data > num)
		if((*root)->left == NULL)
			(*root)->left = createNode(num);
		else
			insert(&((*root)->left), num);
	else if((*root)->right == NULL)
			(*root)->right = createNode(num);
		else
			insert(&((*root)->right), num);
}

void printNode(Node *root){
	printf("data = %d\n", root->data);
	printf("root  position = %p\n", root);
	printf("left  position = %p\n", root->left);
	printf("right position = %p\n", root->right);
}

void find(Node *root, int data){
	if(root == NULL){
		printf("data not found\n");
		return;
	}

	if(root->data == data){
		printNode(root);
		return;
	}

	if(data > root->data)
		find(root->right, data);

	if(data < root->data)
		find(root->left, data);
}

void printTree(Node *root){
	printNode(root);

	if(root->left!=NULL)
		printTree(root->left);
	if(root->right!=NULL)
		printTree(root->right);
}


Node *delete(Node *root, int num, char *found){
	if (root == NULL)
		return root;

	else if(root->data > num)
		root->left = delete(root->left, num, found);

	else if (root->data < num)
		root->right = delete(root->right, num, found);

	else if(root->data == num){
		*found = 'T';
		if(root->left == NULL){
			Node *temp = root->right;
			free(root);
			return temp;
		}

		else if(root->right == NULL){
			Node *temp = root->left;
			free(root);
			return temp;
		}

		if(root->right != NULL && root->left != NULL) {
			Node *min = root->right;
			while(min->left != NULL)
				min = min->left;
			root->data = min->data;
			min->data = num;
			root->right = delete(root->right, num, found);
		}


	}

	return root;
}

void showMenu(){
	printf("command list:\n");
	printf("* i -> insert a node\n");
	printf("* m -> insert multiples nodes\n");
	printf("* r -> remove a node\n");
	printf("* p -> print the tree \n");
	printf("* f -> found a node\n");
	printf("* d -> destroy the tree\n");
	printf("* q -> quit the program\n");
	printf("* h -> show this menu again\n");
}
