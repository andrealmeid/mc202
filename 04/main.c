/* Nome: Andre Figueiredo de Almeida	
 * RA: 164047
 * Laboratorio 04 - Os Caca Pixels */

#include "image.h"
#include <stdio.h>
#include <math.h>
#define  NUM_INTENS 256

typedef struct _pixel{
	int brilho;
	int pos;
	struct _pixel *prox;
} Pixel;

int main(){

	int i, j=0;
	
	/* recebe o endereco da imagem, abre ela, faz uma copia dos valores dos 
	 * brilho para um vetor de struct pixel e fecha a imagem */
	char endereco_imagem[10];	
	scanf("%s", endereco_imagem);
		
	Image *imagem = readImageP5(endereco_imagem);	
	int tamanho_imagem = imagem->n;	
	Pixel *imagem_aux = malloc(tamanho_imagem * sizeof(Pixel));
	
	for(i=0;i<tamanho_imagem;i++){
		imagem_aux[i].brilho = imagem->pixel[i];
		imagem_aux[i].pos=i;
		imagem_aux[i].prox=NULL;
	}
	
	destroyImage(&imagem);

	int c1, c2, max_eq;	
	scanf("%d", &max_eq);
	scanf("%d %d", &c1, &c2);

	/* cria um vetor para fazer o bucket de cada intensidade de brilho */
	Pixel intensidades[NUM_INTENS];
	for(i=0;i<NUM_INTENS;i++)
		intensidades[i].prox=NULL;
	
	/* liga os pixels da imagem em suas intensidade*/
	for(i=tamanho_imagem-1;i>=0;i--){
		if(intensidades[imagem_aux[i].brilho].prox==NULL){
			intensidades[imagem_aux[i].brilho].prox = &imagem_aux[i];			
		} else {
			imagem_aux[i].prox = intensidades[imagem_aux[i].brilho].prox;
			intensidades[imagem_aux[i].brilho].prox = &imagem_aux[i];
		}
	}
	
	/* ordena a imagem pela intensidade de brilho */	
	Pixel *aux;
	Pixel *imagem_ordernada = malloc(tamanho_imagem * sizeof(Pixel));
	for(i=0;i<NUM_INTENS;i++){
		 if(intensidades[i].prox!=NULL){			 
			 aux = intensidades[i].prox;			 
			 do{				 
				imagem_ordernada[j] = *aux;
				aux = aux->prox; 
				j++;
			 } while (aux!=NULL);			 
		 }
	}

	int intervalo = ceil(tamanho_imagem/(max_eq+1));
	
	/* equaliza a imagem */
	int k;
	for(i=0, k=0; i<max_eq && k<tamanho_imagem; i++)
		for(j=0;j<=intervalo;j++, k++)
			imagem_ordernada[k].brilho=i; 
	
   /* imprime a localizacao dos pixels contidos em [c1,c2] */
	for(i=(intervalo+1)*c1;
		i<tamanho_imagem && imagem_ordernada[i].brilho<=c2; i++)
			printf("%d ", imagem_ordernada[i].pos);	
	printf("\n");	
	
	free(imagem_aux);
	free(imagem_ordernada);
	
	return 0;
}