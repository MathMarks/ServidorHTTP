
#include "Servidor.h"
#include <stdio.h>
#include <stdlib.h>

struct Servidor construtor_servidor(int domain, int servico, int protocolo, u_long interface, int porta, int backlog, void (*iniciar)(struct Servidor *servidor))
{

    struct Servidor servidor;

    servidor.domain = domain;
    servidor.servico = servico;
    servidor.protocolo = protocolo;
    servidor.interface = interface;
    servidor.porta = porta;
    servidor.backlog = backlog;

    servidor.endereco.sin_family = domain;
    servidor.endereco.sin_port = htons(porta); // Responsável por converter a porta de int para byte
    servidor.endereco.sin_addr.s_addr = htonl(interface);

    //
    servidor.socket = socket(domain, servico, protocolo);

    if (servidor.socket == 0)
    {

        perror("Falha ao conectar no servidor...\n");
        exit(1);
    }

    // responsável por unir o socket à rede
    // Retorna um valor negativo indicando que não foi capaz de realizar o bind
    if (bind(servidor.socket, (struct sockaddr *)&servidor.endereco, sizeof(servidor.endereco)) < 0)
    {

        perror("Erro ao conectar ao socket....\n");
        exit(1);
    }

    // Servidor entra em listening, aguardando uma requisição do cliente
    if (listen(servidor.socket, servidor.backlog) < 0)
    {

        perror("Falha ao começar a escutar...");
        exit(1);
    }

    servidor.iniciar = iniciar;

    // Cria o "objeto" servidor e retorna para quem  o instanciou, criando o socket, ligando esse socket ao endereço( binding ), e começará a escutar e procurará por erros.
}
