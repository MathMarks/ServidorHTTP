#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Servidor.h"
#include "HTTPRequest.h"

void iniciar(struct Servidor *servidor){

    //Ao chamar essa função, o servidor já está instanciado e pronto para receber a conexão.
    printf("\nRealizando conexão na porta %i", servidor->porta);


    char buffer[BUFFER_SIZE];
    int tamanho_endereco = sizeof(servidor->endereco);
    int novo_socket;
    char *teste = "HTTP/1.0 200 OK\nDate: mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 88\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>ALELUIA<img src=\"index.jpg\"></h1></body></html>";
    while (1)
    {

        printf("_________________________________________________________________");
        // Loop infinito, que estará aceitando novas conexões utilizando a accept
        novo_socket = accept(servidor->socket, (struct sockaddr *)&servidor->endereco, (socklen_t *)&tamanho_endereco);
        read(novo_socket, buffer, BUFFER_SIZE);
        request_handler(buffer,novo_socket);
        //printf("\nreponse navegador\n");
        //printf("%s\n", buffer);
        //printf("\nend response navegador\n");
        //write(novo_socket, teste, strlen(teste));
        //close(novo_socket);
        printf("_________________________________________________________________");

      
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
    struct Servidor servidor = construtor_servidor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT_NUMBER, 10, iniciar);
  
    servidor.iniciar(&servidor);
    
    return 0;
}