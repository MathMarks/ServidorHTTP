#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Servidor.h"
#include "HTTPRequest.h"
#include <arpa/inet.h>
#include <pthread.h>
#include "C_fila.h"
#include <time.h>

#define clear() printf("\033[H\033[J")


pthread_t threads_cliente[THREAD_POOL_SIZE];
pthread_t threads_request;
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

    //Ao chamar essa função, o servidor já está instanciado e pronto para receber as conexões

    int tamanho_endereco = sizeof(servidor->endereco);
    int novo_socket;

    printf("Servidor iniciado com sucesso\nAguardando Conexões\n");

    while (1){


        //printf("\n_____________________________INICIOU REQUISIÇÃO_________________________________\n");

        // Loop infinito, que estará aceitando novas conexões utilizando a accept
        novo_socket = accept(servidor->socket, (struct sockaddr *)&servidor->endereco, (socklen_t *)&tamanho_endereco);

        int *pcliente = (int*) malloc(sizeof(int));
        *pcliente = novo_socket;

        //printf("\n-Socket:: %d:::::\n", *pcliente);
        pthread_mutex_lock(&mutex);
        insere_fila_cliente(pcliente, inet_ntoa(servidor->endereco.sin_addr), ntohs(servidor->endereco.sin_port));
        pthread_mutex_unlock(&mutex);
        
        //request_handler(pcliente);
        //pthread_create(&thread, NULL, request_handler, pcliente);
        
        //write(novo_socket, teste, strlen(teste));
        //printf("\n______________________________FINALIZOU REQUISIÇÃO________________________________\n");
        sleep(3);
    }
}

int main() {

    clear();
    printf("\nServidor HTTP\n");
    printf("\nTrabalho realizado por:\nMatheus Marques\nGiuliana Leon\nThamires Sampaio\n");
    printf("-----------------------------------------------------------------------------\n");
    //Com isso temos uma estrutura de servidor funcionando, agora iniciar a parsing de mensagens.
    sleep(4);
    printf("\nIniciando instanciamento das threads para lidar com clientes\n");
    for(int i = 0; i < THREAD_POOL_SIZE; i++){
        
        if((pthread_create(&threads_cliente[i], NULL, manipula_pool_threads, NULL)) != 0){

            printf("Erro ao instanciar thread n° %d\n",i+1);
            break;

        }

        printf("Thread n°%d instanciada.\r", i+1);
        fflush(stdout);
        sleep(1);

    }
    printf("%d Threads instanciadas com sucesso!\n", THREAD_POOL_SIZE);
    
    printf("Instanciando Thread para lidar com fila de requests\n");
    sleep(2);
    pthread_create(&threads_request, NULL, manipula_fila_request, NULL);

    printf("Thread para lidar com requests instanciada com sucesso\n");
    sleep(5);
    clear();
    //AF_INET - Comunicação ipv4 (AF - Address Family, INET - Internet)
    //SOCK_STREAM - tipo de comunicação
    //SOCK_STREAM, teremos um fluxo de bytes ao invés de ter pacotes(datagrams) separados de dados 
    //0 - Protocolo TCP
    //INADDR_ANY - Esse parâmetro especifica que estamos escutando por qualquer tipo de endereço ip
    //PORT_NUMBER - Porta escolhida
    //10 Backlog
    //iniciar - função inicar teste 
    struct Servidor servidor = construtor_servidor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT_NUMBER, 10, iniciar);
  
    servidor.iniciar(&servidor);
    
    return 0;
}