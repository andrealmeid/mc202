/* Nome: Andre Figueiredo de Almeida
* RA: 164047
* Laboratorio 02 - Matrioshkas Generalizadas */

#include "pilha.h"

void pilha_inserir(int pilha[], int *tam, int num){
	pilha[++*tam] = num;
}

void pilha_remover(int pilha[], int *tam){
	if(tam>=0){
		pilha[*tam] = 0;
		*tam = *tam - 1;
	}
}