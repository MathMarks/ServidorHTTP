
#ifndef Servidor_h
#define Servidor_h

#include <sys/socket.h>
#include <netinet/in.h>

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