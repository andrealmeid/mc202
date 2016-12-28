/* Andre Figueiredo de Almeida *
 * 164047
 * Laboratorio 06 - Queria o MySQL*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* AVL tree node */
typedef struct _node{
	unsigned int data;
	int balance;
	long int position;
	struct _node *left;
	struct _node *right;
} Node;

/* AVL tree root node */
typedef struct _root{
	Node *node;
} Root;

/* Heap array struct */
typedef struct _heap{
	long int *positions;
	int end;
} Heap;

/* Binary file cell struct */
typedef struct _cell{
	unsigned int data;
	char name[1000];
} Cell;

/* Create a new node in AVL Tree */
Node *createNode(int num, long int position);

/* Insert a new node in AVL Tree */
void insertAVL(Node **root, unsigned int num, long int position, int *h);

/* Delete a node in AVL Tree */
void deleteAVL(Node **root, unsigned int num, int *h, Heap *heap);

/* Essas funcoes tratam o balanceamento dos nos da arvore */
void leftIncrease(Node **root, int *h);

void rightIncrease(Node **root, int *h);

void leftDecrease(Node **root, int *h);

void rightDecrease(Node **root, int *h);

/* Funcoes de rotacoes da arvore */
void simpleRightRotation(Node **root);

void simpleLeftRotation(Node **root);

void doubleRightRotation(Node **root);

void doubleLeftRotation(Node **root);

/* Insere um novo elemento no heap e ordena o vetor */
void insertHeap(Heap *heap, long int data);

/* Remove o primeiro elemento do heap */
long int removeHeap(Heap *heap);

/* Percorre a arvore em ordem simetrica e imprime os nos com o 
   balanceamento == balance */
void inOrder(Node *node, int balance, FILE *database);

/* Destroi a arvore desalocando seus nos */
void destroyTree(Node **node);

int main(){
		
	/* Recebe o nome do arquivo e abre ele */
	char file_name[13];
	scanf("%s", file_name);
	FILE *database = fopen(file_name, "r+b");

	/* Recebe o numero de operacoes da entrada e cria um vetor com
	   esse tamanho */
	int num_operations;
	scanf("%d", &num_operations);

	Heap heap;
	heap.positions = malloc(num_operations * sizeof(long int));
	heap.end = -1;

	/* cria a raiz da arvore AVL e as variaveis auxiliares */
	Root root;
	root.node = NULL;

	unsigned int data;
	long int position=0; // posicao da struct no arquivo binario
	int h = 0; // flag de aviso de aumento de altura
	
	while(fread(&data, sizeof(unsigned int), 1, database)){
		insertAVL(&root.node, data, position, &h);
		fseek(database, sizeof(char) * 1000, SEEK_CUR);
		position += sizeof(Cell);
	}

	char cmd; // cmd = comando
	Cell new; // nova celula do arquivo binario

	while(num_operations > 0){
		scanf(" %c", &cmd);	
		switch(cmd){
			/* insersao de um novo no na arvore e celula no arquivo */
			case 'i':
				scanf("%u ", &data);
				new.data = data;
				fgets(new.name, 1000, stdin);
				/* substitiu o \n por \0 */
	            char *lineBreak = strchr(new.name,'\n');
	            *lineBreak = '\0';
	            /* verifica se o Heap esta vazio, caso contrario pega uma 
	            posicao do heap e a usa como posicao da insercao no arquivo */
				if(heap.end == -1){
					insertAVL(&root.node, data, position, &h);
					position += sizeof(Cell);				
					fseek(database, 0, SEEK_END);
				} else {
					long int aux = removeHeap(&heap);					
					insertAVL(&root.node, data, aux, &h);					
					fseek(database, aux, SEEK_SET);
				}
				/* escreve no arquivo */				
				fwrite(&new, sizeof(Cell), 1, database);				
				break;

			case 'r':
			/* remove um no da arvore e guarda sua posicao no heap */				
				scanf("%u", &data);
				deleteAVL(&root.node, data, &h, &heap);						
				break;	
		}

		num_operations--;
	}

	/* recebe o balanceamento da entrada e chama a funcao para imprimir */
	int node_balance;
	scanf("%d", &node_balance);
	inOrder(root.node, node_balance, database);

	/* desaloca as memorias alocadas e fecha o arquivo */
	free(heap.positions);
	destroyTree(&root.node);
	fclose(database);

	return 0;
}

Node *createNode(int num, long int position){
	Node *node = malloc(sizeof(Node));
	node->data  = num;
	node->position = position;
	node->balance = 0;
	node->left  = NULL;
	node->right = NULL;
	
	return node;
}

void simpleRightRotation(Node **root){
	Node *B = (*root)->left;

	(*root)->left = B->right;
	B->right = (*root);
	(*root) = B;
}

void simpleLeftRotation(Node **root){
	Node *B = (*root)->right;

	(*root)->right = B->left;
	B->left = (*root);
	(*root) = B;
}	

void doubleRightRotation(Node **root){
	Node *B = (*root)->left;
	Node *C = B->right;

	B->right = C->left;
	(*root)->left = C->right;
	C->left = B;
	C->right = (*root);
	(*root) = C;

}

void doubleLeftRotation(Node **root){
	Node *B = (*root)->right;
	Node *C = B->left;

	B->left = C->right;
	(*root)->right = C->left;
	C->right = B;
	C->left = (*root);
	(*root) = C;
}

void leftIncrease(Node **root, int *h){
	(*root)->balance--;

	switch((*root)->balance){
		case 0:
			*h = 0;
			return;
		case -1:
			return;
		case -2:
			if((*root)->left->balance == -1){
				simpleRightRotation(root);
				(*root)->balance = 0;
				(*root)->right->balance = 0;
			}
			else{
				doubleRightRotation(root);
				if((*root)->balance == 0){
					(*root)->right->balance = 0;
					(*root)->left->balance = 0;
				} else if((*root)->balance == 1){
					(*root)->right->balance = 0;
					(*root)->left->balance = -1;
				} else {
					(*root)->right->balance = 1;
					(*root)->left->balance = 0;
				}
			}
			(*root)->balance = 0;
			*h = 0;
			return;
	}
}

void rightIncrease(Node **root, int *h){
	(*root)->balance++;
	
	switch((*root)->balance){
		case 0:
			*h = 0;
			return;
		case 1:	
			return;
		case 2:
			if((*root)->right->balance == 1){
				simpleLeftRotation(root);
				(*root)->balance = 0;
				(*root)->left->balance = 0;
			}
			else{
				doubleLeftRotation(root);
				if((*root)->balance == 0){
					(*root)->right->balance = 0;
					(*root)->left->balance = 0;
				} else if((*root)->balance == 1){
					(*root)->right->balance = 0;
					(*root)->left->balance = -1;
				} else {
					(*root)->right->balance = 1;
					(*root)->left->balance = 0;
				}
			}
			
			(*root)->balance = 0;
			*h = 0;
			return;
	}
}

void leftDecrease(Node **root, int *h){
	(*root)->balance++;
	
	switch((*root)->balance){
		case 0:
			return;
		case 1:	
			*h = 0;
			return;
		case 2:
			if((*root)->right->balance == -1){
				doubleLeftRotation(root);

				if((*root)->balance == -1){
					(*root)->left->balance = 0;
					(*root)->right->balance = 1;
				} else if((*root)->balance == 1){
					(*root)->left->balance = -1;
					(*root)->right->balance = 0;
				} else {
					(*root)->left->balance = 0;
					(*root)->right->balance = 0;	
				}

				(*root)->balance = 0;
			}
			else{
				simpleLeftRotation(root);

				if((*root)->right->balance == 0){
					(*root)->balance = -1;
					(*root)->left->balance = 1;
					*h = 0;
				} else {
					(*root)->balance = 0;
					(*root)->left->balance = 0;
				}
			}			
			return;
	}
}

void rightDecrease(Node **root, int *h){
	(*root)->balance--;

	switch((*root)->balance){
		case 0:
			return;
		case -1:
			*h = 0;
			return;
		case -2:
			if((*root)->left->balance == 1){
				doubleRightRotation(root);
				if((*root)->balance == -1){
					(*root)->right->balance = 0;
					(*root)->left->balance = 1;
				} else if((*root)->balance == 1){
					(*root)->right->balance = -1;
					(*root)->left->balance = 0;
				} else {
					(*root)->left->balance = 0;
					(*root)->right->balance = 0;	
				}
				(*root)->balance = 0;
			}
			else{
				simpleRightRotation(root);
				if((*root)->balance == 0){
					(*root)->balance = 1;
					(*root)->right->balance = -1;
					*h = 0;
				} else {
					(*root)->balance = 0;
					(*root)->right->balance = 0;
				}
			}			
			return;
	}
}

void insertAVL(Node **root, unsigned int num, long int position, int *h){
	if((*root) == NULL){
		(*root) = createNode(num, position);
		*h = 1;
	}

	else if(num < (*root)->data){
		insertAVL(&((*root)->left), num, position, h);
		if ((*h)==1){
			leftIncrease(root, h);
		}
	}
	else { 
		insertAVL(&((*root)->right), num,position, h);
		if ((*h)==1){
			rightIncrease(root, h);
		}
	}
}

void deleteAVL(Node **root, unsigned int num, int *h, Heap *heap){
	if((*root) == NULL){
		*h = 0;
		return;
	}

	if((*root)->data < num){
		deleteAVL((&(*root)->right), num, h, heap);
		if(*h==1)
			rightDecrease(root, h);
	}

	else if((*root)->data > num){
		deleteAVL((&(*root)->left), num, h, heap);
		if(*h==1)
			leftDecrease(root, h);
	}

	else if((*root)->data == num){
		if ((*root)->right !=  NULL && (*root)->left != NULL){
			Node *min = (*root)->right;
			int position = (*root)->position;
			while(min->left != NULL)
				min = min->left;
			(*root)->data = min->data;
			(*root)->position = min->position;
			min->data = num;
			min->position = position;

			deleteAVL(&((*root)->right), num, h, heap);
			if(*h==1)
				rightDecrease(root,h);
		} else if ((*root)->right == NULL && (*root)->left != NULL){
			insertHeap(heap, (*root)->position);
			Node *aux = (*root);
			(*root) = (*root)->left;			
			free(aux);
			*h = 1;
		} else if ((*root)->left == NULL && (*root)->right !=  NULL){
			insertHeap(heap, (*root)->position);
			Node *aux = (*root);
			(*root) = (*root)->right;			
			free(aux);
			*h = 1;
		} else {
			insertHeap(heap, (*root)->position);
			free(*root);
			(*root) = NULL;
			*h = 1;
		}

	}
}

void insertHeap(Heap *heap, long int data){

	int actual = ++((*heap).end);
	int root = (actual-1)/2;
	int aux;

	(*heap).positions[actual] = data;

	while(actual >= 0 && (*heap).positions[actual] < (*heap).positions[root]){
		aux = (*heap).positions[root];
		(*heap).positions[root] = (*heap).positions[actual];
		(*heap).positions[actual] = aux;
		actual = root;
		root = (root - 1)/2;
	}
}

long int removeHeap(Heap *heap){
	long int removed = (*heap).positions[0];
	(*heap).positions[0] = (*heap).positions[(*heap).end--];
	int actual = 0;
	int aux;	

	while(2*actual+1 <= (*heap).end){
		if(2*actual+2 <= (*heap).end){
			if((*heap).positions[2*actual+1] < (*heap).positions[2*actual+2]){
				
				if((*heap).positions[actual] > (*heap).positions[2*actual+1]){
					aux = (*heap).positions[actual];
					(*heap).positions[actual] = (*heap).positions[2*actual+1];
					(*heap).positions[2*actual+1] = aux;	
					actual = 2*actual+1;		
				} else 
					break;
			
			} else {

				if((*heap).positions[actual] > (*heap).positions[2*actual+2]){
					aux = (*heap).positions[actual];
					(*heap).positions[actual] = (*heap).positions[2*actual+2];
					(*heap).positions[2*actual+2] = aux;
					actual = 2*actual+2;
				} else
					break;
			}
		} else if ((*heap).positions[actual] > (*heap).positions[2*actual+1]){
			aux = (*heap).positions[actual];
			(*heap).positions[actual] = (*heap).positions[2*actual+1];
			(*heap).positions[2*actual+1] = aux;	
			actual = 2*actual+1;
			break;
		}
	}

	return removed;
} 

void inOrder(Node *node, int balance, FILE *database){
	if(node == NULL)
		return;

	inOrder(node->left, balance, database);

	Cell new;
	if(node->balance == balance){		
		fseek(database, node->position, SEEK_SET);
		fread(&new, sizeof(Cell), 1, database);
		printf("%s %ld\n", new.name, node->position);
	} 

	inOrder(node->right, balance, database);
}

void destroyTree(Node **node){
	if((*node)==NULL)
		return;
	
	destroyTree(&(*node)->right);
	destroyTree(&(*node)->left);

	free(*node);
}