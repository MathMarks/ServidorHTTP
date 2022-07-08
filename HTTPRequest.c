

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>



int request_handler(char * request, int socket){

    char PATH[100], *linhas_da_requisicao[3];


    //Atualiza a PATH variável PATH para o atual do usuário.  
    if (getcwd(PATH, sizeof(PATH)) != NULL) {

       printf("\n\nDiretório atual: %s\n\n\n", PATH);

    } else {
       perror("\n\nErro ao encontrar o diretório.");
       return 1;
    }

    //Extrair o nome do arquivo requisitado pelo cliente de dentro da variável request.
    printf("\n\n------Dentro do request_handler----\n");
    printf("%s\n", request);
    linhas_da_requisicao[0] = strtok(request, " \t\n");
    linhas_da_requisicao[1] = strtok(NULL, " \t");
    linhas_da_requisicao[2] = strtok(NULL, " \t\n");
    printf("\nPrimeira linha da requisição: %s\n", linhas_da_requisicao[0]);
    printf("\nSegunda linha da requisição:  %s\n", linhas_da_requisicao[1]);
    printf("\nTerceira linha da requisição: %s\n", linhas_da_requisicao[2]);


}