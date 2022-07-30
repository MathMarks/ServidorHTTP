
#include "Servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//Construtor 
struct Servidor construtor_servidor(int domain, int servico, int protocolo, u_long interface, int porta, int backlog, void (*iniciar)(struct Servidor *servidor))
{

    printf("\nIniciou a contrução do servidor.\r");
    struct Servidor servidor;

    servidor.domain = domain;
    servidor.servico = servico;
    servidor.protocolo = protocolo;
    servidor.interface = interface;
    servidor.porta = porta;
    servidor.backlog = backlog;

    servidor.endereco.sin_family = domain;
    //htons - "Host to network, short" - 
    servidor.endereco.sin_port = htons(porta); // Responsável por converter a porta de int para byte
    servidor.endereco.sin_addr.s_addr = htonl(interface);//Endereço do cliente

    servidor.socket = socket(domain, servico, protocolo);
    //printf("222");
    if (servidor.socket == 0)
    {
        //Caso a função retorne um valor diferente de zero, significa que houve uma falha durante a criação desse socket e o erro é disparado no console
        perror("Falha ao conectar no servidor...\n");
        exit(1);
    }

    // responsável por unir o socket à rede
    // Retorna um valor negativo indicando que não foi capaz de realizar o bind
    if (bind(servidor.socket, (struct sockaddr *)&servidor.endereco, sizeof(servidor.endereco)) < 0)
    {

        //Caso a função retorne um valor negativo, significa que houve uma falha durante o processo e esse erro é disparado no terminal
        perror("Erro ao conectar ao socket....\n");
        exit(1);
    }

    // Servidor entra em listening, aguardando uma requisição do cliente
    if (listen(servidor.socket, servidor.backlog) < 0)
    {
        //Caso a função retorne um valor negativo, significa que houve uma falha durante o processo e esse erro é disparado no terminal
        perror("Falha ao começar a escutar...");
        exit(1);
    }
    
    printf("Servidor criado com sucesso...........\n");
    sleep(2);
    printf("Porta em uso pelo servidor:\033[35m %i \e[0;37m\n",porta);
    servidor.iniciar = iniciar;

    
    return servidor;
    // Cria o "objeto" servidor e retorna para quem  o instanciou, criando o socket, ligando esse socket ao endereço( binding ), e começará a escutar e procurará por erros.
}
