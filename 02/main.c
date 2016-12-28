/* Nome: Andre Figueiredo de Almeida
* RA: 164047
* Laboratorio 02 - Matrioshkas Generalizadas */

#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"
#include "matrioshka.h"

int main(){

	int num_mkvs, *mkv=NULL, tam_pilha_mkv;
	int *aux=NULL, tam_pilha_aux; 
	int i, boneca, fechou, primeira_boneca;

	scanf("%d", &num_mkvs);

	while(num_mkvs!=0){

		resetar_variaveis(&fechou, &tam_pilha_mkv, 
			&tam_pilha_aux, &mkv, &aux, num_mkvs/2);

		for(i=0;i<num_mkvs;i++){	
			
			scanf("%d", &boneca);

			fechou -= verifica_primeira(i, &primeira_boneca, boneca, num_mkvs);

			/* bonecas que abrem */
			if(boneca<0){
				/* verifica se uma boneca > que a anterior vai ser aberta */
				if(tam_pilha_mkv>=0 && mkv[tam_pilha_mkv]>boneca){				
					fechou=0;			
				} else {
					inserir_bonecas(mkv, aux, 
							&tam_pilha_mkv, &tam_pilha_aux, boneca);
				}
			/* bonecas que fecham */
			} else {			
				if(boneca*-1==mkv[tam_pilha_mkv]){	
					remover_bonecas(mkv, aux, &tam_pilha_mkv, &tam_pilha_aux);
					/* verifica se a boneca nao estorou o limite de tamanho */
					if(tam_pilha_aux>=0){
						aux[tam_pilha_aux]+=boneca;
						if (aux[tam_pilha_aux]>=0){
							fechou = 0;
						}
					}
				/* esse else eh verdadeiro quando se tenta fechar uma 
				   boneca com uma parte errada */
				} else {
					fechou=0;
				}
			}

		}

		scanf("%d", &num_mkvs);
		imprimir_resultado(fechou);
	}

	free(mkv);
	free(aux);

	return 0;
}