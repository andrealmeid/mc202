/* Nome: Andre Figueiredo de Almeida
* RA: 164047
* Laboratorio 01 - O problema do acesso à lista */

#include <stdio.h>
#include <stdlib.h>

/* STRUCS, DEFINIÇÕES E CABEÇALHOS DAS SUAS FUNÇÕES DA TAD */
typedef struct _cel {
    int cont;
    struct _cel *prox;
} Cel;

/* metodo para acessar Move-To-Front */
void acessar_mtf(Cel **p, int aux, int *cost);
/* metodo para acessar Transpose */
void acessar_tr(Cel **p, int num, int *cost);

void inserir_final_lista(Cel *p, int num, int *cost);

void imprimir_lista(Cel *p);

void inserir_inicio_lista(Cel **p, int num);

void remover_celula(Cel **p, int num, int *cost);

void limpar_lista(Cel *p);

int main() {   
    /* n = numero de itens; r = numero de instrucoes */
    int  n, r;
    /* itens guarda os itens iniciais das listas; num atua como auxiliar */
    int *itens, i, num;
    /* costs guardam os custos do MTF e do TR; lixo serve como parametro para 
     * funcoes que nao precisam calcular o custo */
    int costM=0, costT=0, lixo=0;
    /* lista do MTF e do TR */
    Cel *m, *t;
    char c;
    
    m = NULL;
    t = NULL;
    
    scanf("%d %d", &n, &r);
    
    itens = malloc(n * sizeof(int));
    
    for(i=0;i<n;i++)
        scanf("%d", &itens[i]);    
    
    /* cria o ponteiro inicial */
    inserir_inicio_lista(&m, itens[0]);
    inserir_inicio_lista(&t, itens[0]);

    /* cria a lista com os itens da entrada */    
    for(i=1;i<n;i++){
        inserir_final_lista(m, itens[i], &lixo);
        inserir_final_lista(t, itens[i], &lixo);
    }                
    
    /*as instrucoes sao recebidas e executadas ao vivo para as duas listas*/
    for(i=0;i<r;i++){
        scanf(" %c %d", &c, &num);       
        switch(c){
            case 'a':                    
                acessar_mtf(&m, num, &costM);
                acessar_tr(&t, num, &costT);
                break;
            case 'i':                    
                inserir_final_lista(m, num, &costM);
                inserir_final_lista(t, num, &costT);
                acessar_tr(&t, num, &lixo);
                break;
            case 'r':                    
                remover_celula(&m, num, &costM);
                remover_celula(&t, num, &costT);
                break;
        }               
    }
        
    /* impressao dos valores */
    printf("%d\n", costM);
    imprimir_lista(m);
    printf("%d\n", costT);
    imprimir_lista(t);
    
    /* desaloca as listas */
    limpar_lista(m);
    limpar_lista(t);
    
    return 0;
}

void remover_celula(Cel **p, int num, int *cost){
    Cel *anterior = NULL, *proximo = (*p);    
    
    int pos=1;
    
    while(proximo->cont != num){
        anterior = proximo;
        proximo = proximo -> prox;
        pos++;
    }

    if(proximo==NULL){
        anterior->prox=NULL;
    }else if(anterior == NULL){
        (*p) = (*p)->prox;
    } else {
        anterior->prox = proximo->prox;
    }
        
    *cost += pos;
        
    free(proximo);   
}

void acessar_tr(Cel **p, int num, int *cost){
    
    int pos=1, aux;
    Cel (*atual)=(*p), *anterior;
    
    while(atual->cont!=num){
        anterior = atual;
        atual = atual->prox;
        pos++;
    }
    
    if(pos==2){
        aux = (*p)->cont;
        (*p)->cont = atual->cont;
        atual->cont = aux;
    } else 
        if(pos>2){
        aux = anterior->cont;
        anterior->cont = atual->cont;
        atual->cont = aux;
    }
    
    *cost += pos;
}

void acessar_mtf(Cel **p, int num, int *cost){

    int pos=1, lixo=0;
    Cel (*atual)=(*p);
    
    while (atual->cont != num){ 
        atual = atual->prox;
        pos++;
    }   
    
    remover_celula(p, num, &lixo);
    inserir_inicio_lista(p, num);
       
    *cost += pos;
    
}

void inserir_final_lista(Cel *p, int num, int *cost){
    
    Cel *pont = malloc(sizeof(Cel));
    Cel  *atual=p, *penultimo;
    int pos=1;    
    
    while (atual != NULL){  
                penultimo = atual;
                atual = atual->prox;
                pos++;
    }   
    
    pont->prox = NULL;
    pont->cont = num;
    
    penultimo->prox = pont;
    
    *cost += pos;
}

void inserir_inicio_lista(Cel **p, int num){

    Cel *pont = malloc(sizeof(Cel));
          
    pont->prox = (*p);
    pont->cont = num;
    
    (*p) = pont;    
}

void imprimir_lista(Cel *p){
    Cel *atual=p;
    
    while (atual != NULL){  
        printf("%d ", atual->cont);
        atual = atual->prox;
    }   
        printf("\n");
    
}

void limpar_lista(Cel *p){
    Cel *atual=p, *anterior;
    
    if (atual != NULL){
        if(atual->prox == NULL) {
            free(p);
        } else {
            anterior = p;
            atual = p->prox;
            while ( atual != NULL ) {
                free(anterior);
                anterior = atual;
                atual = atual->prox;
            }
            free(anterior);
        }        
    }
}