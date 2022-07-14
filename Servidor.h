//Estruturação da struct Servidor 
//Estruturação da função contrutor_servidor


#ifndef Servidor_h
#define Servidor_h

#include <sys/socket.h>
#include <netinet/in.h>


#define PORT_NUMBER 80 //Porta utilizada
#define BUFFER_SIZE 4000 //Tamanho do buffer
#define THREAD_POOL_SIZE 10 //Quantidade de threads possíveis simultâneamente

struct Servidor
{
    int domain;
    int servico;
    int protocolo;
    u_long interface;
    int porta;
    int backlog;
    int socket;

    struct sockaddr_in endereco;

    void (*iniciar)(struct Servidor *servidor);
};

struct Servidor construtor_servidor(int domain, int servico, int protocolo, u_long interface, int porta, int backlog, void (*iniciar)(struct Servidor *servidor));

#endif