

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Servidor.h"

void iniciar(struct Servidor *servidor)
{

    printf("Esperando uma Conexão");
    char buffer[40000];
    int tamanho_endereco = sizeof(servidor->endereco);
    int novo_socket;
    char *teste = "HTTP/1.0 200 OK\nDate: mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 88\nContent-Type: text/html\nConnection: Closed\n<html><body><h1>ALELUIA</h1></body></html>";

    while (1)
    {

        printf("%s\n", buffer);
        printf("1 -\n");
        novo_socket = accept(servidor->socket, (struct sockaddr *)&servidor->endereco, (socklen_t *)&tamanho_endereco);
        read(novo_socket, buffer, 40000);
        write(novo_socket, teste, strlen(teste));
        close(novo_socket);
        printf("2 -\n");
    }
}

int main()
{
    printf("main 1\n");
    struct Servidor servidor = construtor_servidor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, iniciar);
    printf("main 2\n");
    servidor.iniciar(&servidor);

    return 0;
}