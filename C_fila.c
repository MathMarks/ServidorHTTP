

#include <stdlib.h>
#include "C_fila.h"

node_c * ini_cliente = NULL;
node_c * fim_cliente = NULL;

node_r * ini_request = NULL;
node_r * fim_request = NULL;


void insere_fila_cliente(int *socket){

    node_c * novo_no = malloc(sizeof(node_c));
    novo_no->socket = socket;
    novo_no->prox = NULL;

    if(fim_cliente == NULL){

        ini_cliente = novo_no;

    } else {

        fim_cliente->prox = novo_no;

    }

    fim_cliente = novo_no;

}

int * retira_fila_cliente(){

    if(ini_cliente == NULL){

        return NULL;

    } else {

        int * prox_socket = ini_cliente->socket;
        node_c * aux = ini_cliente;
        ini_cliente = aux->prox;

        if(ini_cliente == NULL) fim_cliente = NULL;

        free(aux);

        return prox_socket;

    }

}

requestInfo * cria_request(char * buffer, int * socket){

    requestInfo * req = malloc(sizeof(requestInfo));
    req->buffer = buffer;
    req->socket = socket;

    return req;

}

void insere_fila_request(int *socket, char * buffer){

    node_r * novo_no = malloc(sizeof(node_r));
    requestInfo * novo_request = malloc(sizeof(requestInfo));
    novo_no->request = novo_request;
    novo_no->request->socket = socket;
    novo_no->request->buffer = buffer;
    novo_no->prox = NULL;

    if(fim_request == NULL){

        ini_request = novo_no;

    } else {

        fim_request->prox = novo_no;

    }

    fim_request = novo_no;

}

requestInfo * retira_fila_request(){

    if(ini_request == NULL){

        return NULL;

    } else {

        requestInfo * prox_request = ini_request->request;
        node_r * aux = ini_request;
        ini_request = aux->prox;

        if(ini_request == NULL) fim_request = NULL;

        free(aux);

        return prox_request;

    }

    return NULL;

}