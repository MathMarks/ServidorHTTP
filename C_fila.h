



#ifndef C_fila_h
#define C_fila_h

#include <pthread.h>
//#include "HTTPRequest.h"


struct node_c
{
    struct node_c* prox;
    int *socket;
    char * ip_cliente;
    int porta_cliente;
};

struct node_r
{
    struct node_r* prox;
    struct requestInfo * request;
};

struct requestInfo{

    int * socket;
    char * buffer;
};

typedef struct node_c node_c;
typedef struct node_r node_r;
typedef struct requestInfo requestInfo;

void insere_fila_cliente(int *socket, char * ip_cliente, int porta_cliente);
int * retira_fila_cliente();
requestInfo * cria_request(char * buffer, int * socket);
void insere_fila_request(int *socket, char * buffer);
requestInfo * retira_fila_request();




#endif // !C_fila_h

