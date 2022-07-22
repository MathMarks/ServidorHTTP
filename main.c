#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Servidor.h"
#include "HTTPRequest.h"
#include <pthread.h>
#include "C_fila.h"
#include <time.h>

pthread_t threads_cliente[THREAD_POOL_SIZE];
pthread_t threads_request[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void * manipula_pool_threads(void * arg){

    while(1){

        pthread_mutex_lock(&mutex);
        int *c_socket = retira_fila_cliente();
        pthread_mutex_unlock(&mutex);
        
        if(c_socket != NULL){

             request_handler(c_socket);

        }
    }
}


void iniciar(struct Servidor *servidor){

    //Ao chamar essa função, o servidor já está instanciado e pronto para receber a conexão.

    int tamanho_endereco = sizeof(servidor->endereco);
    int novo_socket;

    while (1){


        printf("\n_____________________________INICIOU REQUISIÇÃO_________________________________\n");

        // Loop infinito, que estará aceitando novas conexões utilizando a accept
        novo_socket = accept(servidor->socket, (struct sockaddr *)&servidor->endereco, (socklen_t *)&tamanho_endereco);

        int *pcliente = (int*) malloc(sizeof(int));
        *pcliente = novo_socket;

        printf("\n-Socket:: %d:::::\n", *pcliente);
        pthread_mutex_lock(&mutex);
        insere_fila_cliente(pcliente);
        pthread_mutex_unlock(&mutex);
        
        //request_handler(pcliente);
        //pthread_create(&thread, NULL, request_handler, pcliente);
        
        //write(novo_socket, teste, strlen(teste));
        printf("\n______________________________FINALIZOU REQUISIÇÃO________________________________\n");
      sleep(1000);
    }
}

int main() {

    //Com isso temos uma estrutura de servidor funcionando, agora iniciar a parsing de mensagens.

    //AF_INET - Comunicação ipv4 (AF - Address Family, INET - Internet)
    //SOCK_STREAM - tipo de comunicação
    //SOCK_STREAM, teremos um fluxo de bytes ao invés de ter pacotes(datagrams) separados de dados 
    //0 - Protocolo TCP
    //INADDR_ANY - Esse parâmetro especifica que estamos escutando por qualquer tipo de endereço ip
    //PORT_NUMBER - Porta escolhida
    //10 Backlog
    //iniciar - função inicar teste 
    for(int i = 0; i < THREAD_POOL_SIZE; i++){

        pthread_create(&threads_cliente[i], NULL, manipula_pool_threads, NULL);

    }
    for(int i = 0; i < THREAD_POOL_SIZE; i++){

        pthread_create(&threads_request[i], NULL, manipula_fila_request, NULL);

    }

    struct Servidor servidor = construtor_servidor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT_NUMBER, 10, iniciar);
  
    servidor.iniciar(&servidor);
    
    return 0;
}