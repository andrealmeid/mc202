/* Andre Figueiredo de Almeida
   164047
   Laboratorio 08 - Corra por sua vida */

#include <stdio.h>
#include <stdlib.h>

/* cores dos grafos correnpondentes aos acesso */
typedef enum { BRANCO, CINZA, PRETO } cor;

/* estrutura de grafo */
typedef struct _celula{
  int temp;
  int cor;
  int x,y;
} celula;

/* estrutura do heap */
typedef struct _heap{
  celula *positions;
  int end;
} Heap;

/* Insere um novo elemento no heap e ordena o vetor */
void insertHeap(Heap *heap, celula data);

/* Remove o primeiro elemento do heap */
celula removeHeap(Heap *heap);

int main(){

  int linhas, colunas, i, j;
  scanf("%d %d", &linhas, &colunas);

  /* matriz de grafos que se transforma em matriz de custos */
  celula **custo;
  custo = malloc(sizeof(celula*) * linhas);
  for(i=0; i < linhas; i++)
    custo[i] = malloc(sizeof(celula) * colunas);

  for(i=0; i < linhas; i++){
    for(j=0; j < colunas; j++){
      scanf("%d\n", &custo[i][j].temp);
      custo[i][j].cor = BRANCO;
      custo[i][j].x = i;
      custo[i][j].y = j;
    }
  }

  int entradaX, entradaY;
  scanf("%d %d", &entradaX, &entradaY);

  Heap heap;
 	heap.positions = malloc(linhas * colunas * sizeof(celula));
	 heap.end = -1;

  /* insere no heap o primeiro grafo, a entrada da sala */
  insertHeap(&heap, custo[entradaX][entradaY]);
  custo[entradaX][entradaY].cor = CINZA;

  /* enquanto existirem grafos brancos, insere eles no heap e compara o seu
     custo com os dos vizinhos */
  while (heap.end >= 0) {
    celula atual = removeHeap(&heap);
    custo[atual.x][atual.y].cor = PRETO;

    for(i = atual.x-1; i <= atual.x+1; i++)
      if(i >= 0 && i < linhas)

        for(j = atual.y-1; j <= atual.y+1; j++)
          if(j >= 0 && j < colunas)

            if(!(i == atual.x && j == atual.y)){
              if(custo[i][j].cor != PRETO){
                if(atual.temp > custo[i][j].temp)
                  custo[i][j].temp = atual.temp;
                if(custo[i][j].cor == BRANCO) {
                 insertHeap(&heap, custo[i][j]);
                 custo[i][j].cor = CINZA;
                }
              }
            }
  }

  int num_saidas;
  scanf("%d", &num_saidas);

  /* compara a temperatura das saidas para chegar na melhor */
  int saida_ideal = 201;
  for(i = 0; i < num_saidas; i++){
    int saidaX, saidaY;
    scanf("%d %d", &saidaX, &saidaY);
    if(custo[saidaX][saidaY].temp < saida_ideal)
     saida_ideal = custo[saidaX][saidaY].temp;
  }

  printf("%d\n", saida_ideal);

  free(heap.positions);
  for(i=0; i < linhas; i++)
    free(custo[i]);
  free(custo);

  return 0;
}

void insertHeap(Heap *heap, celula data){

  int actual = ++((*heap).end);
  int root = (actual-1)/2;
  celula aux;

  (*heap).positions[actual] = data;

  while(actual >= 0 &&
        (*heap).positions[actual].temp < (*heap).positions[root].temp)
  {
    aux = (*heap).positions[root];
    (*heap).positions[root] = (*heap).positions[actual];
    (*heap).positions[actual] = aux;
    actual = root;
    root = (root - 1)/2;
  }
}

celula removeHeap(Heap *heap){
  celula removed = (*heap).positions[0];
  (*heap).positions[0] = (*heap).positions[(*heap).end--];
  int actual = 0;
  celula aux;
  while(2*actual+1 <= (*heap).end){
    if(2*actual+2 <= (*heap).end){
      if((*heap).positions[2*actual+1].temp < (*heap).positions[2*actual+2].temp){

        if((*heap).positions[actual].temp > (*heap).positions[2*actual+1].temp){
          aux = (*heap).positions[actual];
          (*heap).positions[actual] = (*heap).positions[2*actual+1];
          (*heap).positions[2*actual+1] = aux;
          actual = 2*actual+1;
        } else
           return removed;

      } else {

        if((*heap).positions[actual].temp > (*heap).positions[2*actual+2].temp){
          aux = (*heap).positions[actual];
          (*heap).positions[actual] = (*heap).positions[2*actual+2];
          (*heap).positions[2*actual+2] = aux;
          actual = 2*actual+2;
        } else
           return removed;
      }
    } else
      if ((*heap).positions[actual].temp > (*heap).positions[2*actual+1].temp){
      aux = (*heap).positions[actual];
      (*heap).positions[actual] = (*heap).positions[2*actual+1];
      (*heap).positions[2*actual+1] = aux;
      actual = 2*actual+1;
      return removed;
    } else
      return removed;
  }

  return removed;
}
