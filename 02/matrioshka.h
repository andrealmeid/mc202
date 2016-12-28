/* Nome: Andre Figueiredo de Almeida
* RA: 164047
* Laboratorio 02 - Matrioshkas Generalizadas */

#ifndef MATRIOSHKA_H
#define MATRIOSHKA_H

#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

/* verifica se a primeira boneca que abre eh a ultima que fecha */
int verifica_primeira(int i, int *primeira_boneca, int boneca, int num_mkvs);

/* reinicia as variaveis para o proximo loop */
void resetar_variaveis(int *f, int *tam_m, int *tam_a, int **m, int **a, int tam);

/* imprime se eh ou nao eh matrioshka apartir de uma flag */
void imprimir_resultado(int x);

/* insere as bonecas nas listas */
void inserir_bonecas(int *m, int *a, int *tam_m, int *tam_a, int boneca);

/* remove as bonecas das listas */
void remover_bonecas(int *m, int *a, int *tam_m, int *tam_a);

#endif