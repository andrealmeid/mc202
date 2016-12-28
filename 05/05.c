/* Nome: Andre Figueiredo de Almeida
 * RA: 164047
 * Laboratorio 05 - Calculadora Pseudocientífica */

#include <stdio.h>
#include <stdlib.h>

typedef struct _arvore{
	
	int elem;
	struct _arvore *esq;
	struct _arvore *dir;
	
} NoArvore, RaizArvore;

/* funcao que cria um novo no na arvore */
NoArvore *criarNo(int elem, NoArvore *esq, NoArvore *dir);

/* funcao recursiva que cria e preenche a arvore */
RaizArvore *criarArvore();

/* realiza as operacoes matematicas da arvore */
int calcular(RaizArvore *T);

/* desaloca a arvore */
void destroiArvore(RaizArvore *T);

int main(){
	
	RaizArvore *T = criarArvore();

	printf("%d\n", calcular(T));
	
	destroiArvore(T);

	return 0;
}

NoArvore *criarNo(int elem, NoArvore *esq, NoArvore *dir){
	NoArvore *no = malloc(sizeof(NoArvore));
	
	no->elem = elem;
 	no->dir = dir;
 	no->esq = esq;	

	return no;
}

RaizArvore *criarArvore(){
	char c;
	scanf("%c", &c);
	
	/* se esta fechando um no, retorna null */
	if(c == ')')
		return NULL;
	
	NoArvore *esq, *dir;
	int num = 0;	

	/* cria a arvore da esquerda */
	esq = criarArvore();

	/* define a raiz; se a arvore estiver vazia, o scanf recebera EOF e quebrara
	 * o loop; se a arvore nao for vazia, ele corre o buffer ate achar um int */
	while(!scanf("%d", &num))
		scanf("%c", &c);
	scanf("%c", &c);
		
	dir = criarArvore();
		
	return criarNo(num, esq, dir);
}

int calcular(RaizArvore *T){
	if(T->esq==NULL)
		return T->elem;
	
	int resultado;
	switch(T->elem){
		/* funcao soma */
		case -1:
			resultado = calcular(T->esq) + calcular(T->dir);
			break;
			
		/* funcao subtracao */
		case -2:
			resultado = calcular(T->esq) - calcular(T->dir);
			break;
			
		case -3:
		/* funcao multiplicacao */
			resultado = calcular(T->esq) * calcular(T->dir);
			break;
			
		/* funcao divisao */
		case -4:
			resultado = calcular(T->esq) / calcular(T->dir);
			break;
	}
	
	return resultado;
}

void destroiArvore(RaizArvore *T){
	if(T!=NULL){
		destroiArvore(T->esq);
		destroiArvore(T->dir);
		free(T);
	}
}