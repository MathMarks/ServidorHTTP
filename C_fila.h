



#ifndef C_fila_h
#define C_fila_h

#include <pthread.h>


struct node
{
    struct node* prox;
    int *socket;
};

typedef struct node node_t;

void insere_fila(int *socket);
int * retira_fila();



#endif // !C_fila_h

