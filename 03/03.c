/* Nome: Andre Figueiredo de Almeida
 * RA: 164047
 * Laboratorio 03 - Sistema Único de Saúde */

#include <stdlib.h>
#include <stdio.h>

typedef struct _no{
	int elem;
	struct _no *prox;
	struct _no *ante;
} No;

typedef struct _listaDupla{
	No *inicio;
	No *final;
} ListaDupla;

void criarLista(ListaDupla *lista, int tam);

void emergencia(ListaDupla *lista, int num);

void atendimento(ListaDupla *lista);

void deletarLista(ListaDupla *lista);

int main(){
	
	ListaDupla pacientes;
	char comando;
	/* essa variavel eh reutilizada; em um primeiro momento eh o numero total,
	   depois o numero do paciente que da emergencia */
	int numeroPaciente;
	
	scanf("%d", &numeroPaciente);
	/* o tamanho da lista sera o numero de pacientes */
	criarLista(&pacientes, numeroPaciente);
	
	scanf(" %c", &comando);
	/* o programa roda ate receber o comando 'F' */
	while(comando!='F'){
		switch(comando){
			case 'A':
				atendimento(&pacientes);
				break;
			case 'E':
				scanf("%d", &numeroPaciente);
				emergencia(&pacientes, numeroPaciente);
				break;
		}
		scanf(" %c", &comando);
	}	
	
	/* desalocamento da memoria */
	deletarLista(&pacientes);

	return 0;
}

/* cria uma lista dupla com os pacientes */
void criarLista(ListaDupla *lista, int tam){
	int id=1;
	No *p;
	p = malloc(sizeof(No));
	p->prox = NULL;
	p->ante = NULL;
	p->elem = id++;

	lista->inicio = p;
	lista->final = p;
	
	while(id<=tam){
		p = malloc(sizeof(No));
		p->prox = NULL;
		p->ante = lista->final;			
		p->elem = id++;
		
		p->ante->prox=p;			
		lista->final=p;
	}	
}

/* atende o paciente e coloca ele no final da fila */
void atendimento(ListaDupla *lista){
	No *p = lista->inicio;
	printf("%d\n", p->elem);

	lista->inicio = lista->inicio->prox;
	lista->final->prox = p;
	p->prox = NULL;
	p->ante = lista->final;
	lista->final = p;
}
	
/* busca o paciente e coloca ele no comeco da fila */
void emergencia(ListaDupla *lista, int paciente){
	No *p = lista->inicio;

	/* se for o primeiro elemento, nada acontece */
	if(p->elem != paciente){
		p = p->prox;

		/* encontra o paciente */
		while(p->elem != paciente)		
			p = p->prox;
		
		/* se o elemento estiver no meio da fila */
		if(p->prox != NULL){

			lista->inicio->ante = p;			
			p->ante->prox = p->prox; 
			p->prox->ante = p->ante; 
			p->prox = lista->inicio; 
			p->ante = NULL; 			
			lista->inicio = p; 
		}
		/* se estiver no fim da fila*/
		else {

			p->ante->prox = NULL;
			lista->final = p->ante;			
			p->prox = lista->inicio;
			p->ante = NULL;
			lista->inicio->ante = p;
			lista->inicio = p;
		}		
	}	
}

/* deleta a lista */
void deletarLista(ListaDupla *lista){
	No *p = lista->inicio;
	No *prox = lista->inicio;

	while(prox->prox!=NULL){
		prox=p->prox;
		free(p);
		p = prox;
	}

	free(p);
}