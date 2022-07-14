

#include <stdlib.h>
#include "C_fila.h"

node_t * ini = NULL;
node_t * fim = NULL;


void insere_fila(int *socket){

    node_t * novo_no = malloc(sizeof(node_t));
    novo_no->socket = socket;
    novo_no->prox = NULL;

    if(fim == NULL){

        ini = novo_no;

    } else {

        fim->prox = novo_no;

    }

    fim = novo_no;

}

int * retira_fila(){

    if(ini == NULL){

        return NULL;

    } else {

        int * prox_socket = ini->socket;
        node_t * aux = ini;
        ini = aux->prox;

        if(ini == NULL) fim = NULL;

        free(aux);

        return prox_socket;

    }

}