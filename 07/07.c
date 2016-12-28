/* Andre Figueiredo de Almeida
 * 164047
 * Laboratorio 07 - De Javu */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct _no {
	int *id;
	long int *local;
	int elementos, max_elementos;
	struct _no **filhos;
} no;

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

no* cria(int ordem);

void ordenaVetor(int vet[], long int loc[], int tam);

int verificaOverflow(no *arvore);

int verificaUnderflow(no *arvore);

void trocaId(no *arvore, int pos);

void deletar(no *arvore, int id, int *under, Heap *heap);

void trataUnderflow(no *arvore, int pos);

void insere(no *arvore, int id, long int local, int *over);

void inserePai(no **arvore, int id, long int local);

/* Insere um novo elemento no heap e ordena o vetor */
void insertHeap(Heap *heap, long int data);

/* Remove o primeiro elemento do heap */
long int removeHeap(Heap *heap);

/* Destroi a arvore desalocando seus nos */
void destroyTree(no **node);

void imprimir(no *raiz, int altura, int i, FILE *database);

int main(){

	int ordem;
	scanf("%d", &ordem);
	no *raiz;
	raiz = cria(ordem);

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

	unsigned int id;
	long int local=0; // posicao da struct no arquivo binario

	while(fread(&id, sizeof(unsigned int), 1, database)){
		inserePai(&raiz, id, local);
		fseek(database, sizeof(char) * 1000, SEEK_CUR);
		local += sizeof(Cell);
	}

	char cmd; // cmd = comando
	Cell new; // nova celula do arquivo binario

	while(num_operations > 0){
		scanf(" %c", &cmd);
		switch(cmd){
			/* insersao de um novo no na arvore e celula no arquivo */
			case 'i':
				scanf("%u ", &id);
				new.data = id;
				fgets(new.name, 1000, stdin);
				/* substitiu o \n por \0 */
	            char *lineBreak = strchr(new.name,'\n');
	            *lineBreak = '\0';
	            /* verifica se o Heap esta vazio, caso contrario pega uma
	            posicao do heap e a usa como posicao da insercao no arquivo */
				if(heap.end == -1){
					inserePai(&raiz, id, local);
					local += sizeof(Cell);
					fseek(database, 0, SEEK_END);
				} else {
					long int aux = removeHeap(&heap);
					inserePai(&raiz, id, aux);
					fseek(database, aux, SEEK_SET);
				}
				/* escreve no arquivo */
				fwrite(&new, sizeof(Cell), 1, database);
				break;

			case 'r':
			/* remove um no da arvore e guarda sua posicao no heap */
				scanf("%u", &id);
				int under = FALSE;
				deletar(raiz, id, &under, &heap);
				break;
		}

		num_operations--;
	}

	int altura;
	scanf("%d", &altura);

	imprimir(raiz, altura-1, 0, database);

	return 0;
}

no* cria(int ordem){
	no *novo = malloc(sizeof(no));

	novo->max_elementos = ordem;
	novo->elementos = 0;
	novo->id = malloc(sizeof(no) * ordem + 1);
	novo->local = malloc(sizeof(no) * ordem + 1);
	novo->filhos = calloc(ordem + 2, sizeof(no*));

	return novo;
}

int verificaOverflow(no *arvore){
	if (arvore->elementos > arvore->max_elementos)
		return 1;
	else
		return 0;
}

void ordenaVetor(int vet[], long int loc[], int tam){
	int i, j, aux;
	long int aux2;
	for (i = 1; i < tam; i++) {
					j = i;
					while (j > 0 && vet[j-1] > vet[j]) {
								aux = vet[j];
								vet[j] = vet[j-1];
								vet[j-1] = aux;
								aux2 = loc[j];
								loc[j] = loc[j-1];
								loc[j-1] = aux2;
								j--;
					}
		}
}

void inserePai(no **arvore, int id, long int local){
	int over = FALSE, i, j;
	no *irmao;

	insere((*arvore), id, local, &over);

	if (over){
		no *raiz = cria((*arvore)->max_elementos);
		int mediana = ((*arvore)->max_elementos+1)/2;
		raiz->id[0] = (*arvore)->id[mediana];
		raiz->local[0] = (*arvore)->local[mediana];
		raiz->elementos++;
		irmao = cria(raiz->max_elementos);
		for(j=0, i=mediana+1;i<(*arvore)->elementos;i++, j++){
			irmao->id[j] = (*arvore)->id[i];
			irmao->local[j] = (*arvore)->local[i];
			irmao->filhos[j] = (*arvore)->filhos[i];
		}
		irmao->filhos[j] = (*arvore)->filhos[i];
		irmao->elementos = j;
		(*arvore)->elementos = mediana;
		raiz->filhos[0] = (*arvore);
		raiz->filhos[1] = irmao;
		(*arvore) = raiz;
	}
}

void insere(no *arvore, int id, long int local, int *over){
	int pos, i, j;
	no *irmao;

	if(arvore->filhos[0] != NULL){
			for (pos = 0; pos < arvore->elementos && id > arvore->id[pos]; pos++);
			insere(arvore->filhos[pos], id, local, over);

			if (*over){
				int mediana = (arvore->max_elementos+1)/2;
				int medio = arvore->filhos[pos]->id[mediana];

				irmao = cria(arvore->max_elementos);

				for(i=0, j=mediana+1; j < arvore->filhos[pos]->elementos; i++, j++){
					irmao->id[i] = arvore->filhos[pos]->id[j];
					irmao->local[i] = arvore->filhos[pos]->local[j];
				}

				arvore->filhos[pos]->elementos = j-i-1;
				irmao->elementos = i;
				for(i=0; i <= irmao->elementos; i++)
					irmao->filhos[i] = arvore->filhos[pos]->filhos[mediana+i+1];

			  int a = arvore->elementos;
			  while(a >= pos){
			    arvore->id[a+1] = arvore->id[a];
					arvore->local[a+1] = arvore->local[a];
			    arvore->filhos[a+1] = arvore->filhos[a];
			    a--;
			  }

				arvore->id[pos] = medio;
				arvore->local[pos] = arvore->filhos[pos]->local[mediana];
				arvore->elementos++;

				*over = verificaOverflow(arvore);

				arvore->filhos[pos+1] = irmao;
			}

			return;
	}

	if(arvore->filhos[0] == NULL){
		arvore->id[arvore->elementos] = id;
		arvore->local[arvore->elementos] = local;
		arvore->elementos++;

		ordenaVetor(arvore->id, arvore->local, arvore->elementos);

		*over = verificaOverflow(arvore);
	}
}

int verificaUnderflow(no *arvore){
	if (arvore->elementos < arvore->max_elementos/2)
		return 1;
	else
		return 0;
}

int buscaId(int vet[], int id, int tam){
	int i;
	for(i=0;i<tam;i++)
		if(vet[i] == id)
			return i;
	return -1;
}

void trocaId(no *arvore, int pos){
		no *prox = arvore->filhos[pos];
    while(prox->filhos[prox->elementos] != NULL)
    	prox = prox->filhos[prox->elementos];

    int aux = arvore->id[pos];
    arvore->id[pos] = prox->id[prox->elementos-1];
		arvore->local[pos] = prox->local[prox->elementos-1];
    prox->id[prox->elementos-1] = aux;
}

void deletar(no *arvore, int id, int *under, Heap *heap){
	int pos = buscaId(arvore->id, id, arvore->elementos);
		if (pos == -1){
			if (arvore->filhos[0]!=NULL){
				for(pos = 0; pos < arvore->elementos
						&& id > arvore->id[pos]; pos++);
				deletar(arvore->filhos[pos], id, under, heap);
				if(*under)
					trataUnderflow(arvore, pos);
				*under = verificaUnderflow(arvore);
			}
    } else {
			if (arvore->filhos[pos]!=NULL) {
					trocaId(arvore, pos);
					deletar(arvore->filhos[pos], id, under, heap);
					if (*under)
							trataUnderflow(arvore, pos);
			} else {
					insertHeap(heap, arvore->local[pos]);
					int i;
					for(i=pos; i<arvore->elementos; i++){
							arvore->id[i] = arvore->id[i+1];
							arvore->local[i] = arvore->local[i+1];
						}
					arvore->elementos--;
					*under = verificaUnderflow(arvore);
			}
    }
}

void trataUnderflow(no *arvore, int pos){
    no *folha = arvore->filhos[pos];
    if(pos > 0){
			no *esq = arvore->filhos[pos-1];
      if(esq->elementos-1 >= arvore->max_elementos/2){
				  esq->elementos--;
          folha->id[folha->elementos] = esq->id[esq->elementos];
					folha->local[folha->elementos] = esq->local[esq->elementos];
					return;
			} else {
				esq->id[esq->elementos] = arvore->id[pos-1];
				esq->local[esq->elementos] = arvore->local[pos-1];
				esq->elementos++;
				int i;
				for(i=0; i<folha->elementos; i++){
						esq->id[esq->elementos] = folha->id[i];
						esq->local[esq->elementos] = folha->local[i];
						esq->elementos++;
				}
				free(arvore->filhos[arvore->elementos]);
				arvore->filhos[arvore->elementos] = NULL;
				arvore->elementos--;
				if(arvore->elementos == 0)
						arvore = esq;
			}
    }
		if(arvore->elementos > 1 && pos < arvore->elementos) {
			no *dir = arvore->filhos[pos+1];
			if(dir->elementos-1 >= arvore->max_elementos/2){
		      folha->id[folha->elementos] = dir->id[0];
					folha->local[folha->elementos] = dir->local[0];
					int i;
					for(i=0; i<dir->elementos; i++){
							dir->id[i] = dir->id[i+1];
							dir->local[i] = dir->local[i+1];
					}
				  dir->elementos--;
					return;
				} else {					
						folha->id[folha->elementos] = arvore->id[pos];
						folha->local[folha->elementos] = arvore->local[pos];
						folha->elementos++;
						int i;
						for(i=0; i<dir->elementos; i++){
							folha->id[folha->elementos] = dir->id[i];
							folha->local[folha->elementos] = dir->local[i];
							folha->elementos++;
						}
						for(i=pos; i<arvore->elementos-1; i++){
							arvore->id[i] = arvore->id[i+1];
							arvore->local[i] = arvore->id[i+1];
						}
						free(dir);
						for(i=pos+1; i < arvore->elementos; i++)
								arvore->filhos[i] = arvore->filhos[i+1];
						arvore->filhos[arvore->elementos] = NULL;
						arvore->elementos--;

						if(arvore->elementos == 0)
								arvore = folha;
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

void imprimir(no *raiz, int altura, int i, FILE *database){
	int j;
	if (i < altura){
		for(j=0; j <= raiz->elementos; j++)
			imprimir(raiz->filhos[j], altura, i+1, database);
	} else {
		Cell new;
		for(j=0; j < raiz->elementos; j++){
			fseek(database, raiz->local[j], SEEK_SET);
			fread(&new, sizeof(Cell), 1, database);
			//printf("%s %ld (%d)\n", new.name, raiz->local[j], raiz->id[j]);
			printf("%s %ld\n", new.name, raiz->local[j]);
		}
	}
}
