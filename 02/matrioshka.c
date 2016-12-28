/* Nome: Andre Figueiredo de Almeida
* RA: 164047
* Laboratorio 02 - Matrioshkas Generalizadas */

#include "matrioshka.h"
#define TRUE 1
#define FALSE 0

int verifica_primeira(int i, int *p, int b, int n){
	if(i==0){
		*p=b*-1;
		return FALSE;
	} 

	if(i!=n-1 && b==*p){			
		return TRUE;			
	}

	return FALSE;	
}

void resetar_variaveis(int *f, int *tam_m, int *tam_a, int **m, int **a, int tam){
		free(*m);
		free(*a);
		*f = 1;
		*tam_m = -1;
		*tam_a = -1;
		(*m) = malloc(tam * sizeof(int));
		(*a) = malloc(tam * sizeof(int));	
}

void imprimir_resultado(int x){
	if(x==1)
		printf("Eh Matrioshka.\n");
	else
		printf("Nao eh Matrioshka.\n");
}

void inserir_bonecas(int *m, int *a, int *tam_m, int *tam_a, int boneca){
	pilha_inserir(m, tam_m, boneca);
	pilha_inserir(a, tam_a, boneca);
}

void remover_bonecas(int *m, int *a, int *tam_m, int *tam_a){
	pilha_remover(m, tam_m);
	pilha_remover(a, tam_a);
}
