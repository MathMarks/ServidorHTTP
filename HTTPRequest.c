
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>  
#include <netinet/in.h>
#include <fcntl.h>
#include "Servidor.h"

char PATH[100];

void * request_handler(void * c_socket){

   printf("\nSocket::%d\n", * ((int*)c_socket));

   int socket = * ((int*)c_socket); 
   free(c_socket);
   
   char buffer[BUFFER_SIZE];

   read(socket, buffer, BUFFER_SIZE);
   printf("\nInicio Requisição Navegador\n");
   printf("%s\n", buffer);
   printf("\nFinal Requisição navegador\n");

   char *infos_da_requisicao[3];


    //Atualiza a variável PATH para o atual PATH do usuário.  
    if (getcwd(PATH, sizeof(PATH)) != NULL) {

       //Mostra no console qual o diretório atual do servidor, apenas para debug. 
       //printf("\n\nDiretório atual: %s\n\n\n", PATH);

    } else {
       perror("\nErro ao encontrar o diretório.");
       return NULL;
       
    }

    //Extrair o nome do arquivo requisitado pelo cliente de dentro da variável request.
    printf("\n------Dentro do request_handler----\n");

    //printf("%s\n", request); //Imprime toda a requisição do cliente e informações sobre ele

    //Captura o metodo, que sempre será o GET no caso dessa implementação
    infos_da_requisicao[0] = strtok(buffer, " \t\n");
    //Captura o arquivo que o cliente está solicitando, html ou jpg até o momento
    infos_da_requisicao[1] = strtok(NULL, " \t");
    //captura a versão do HTTP em uso
    infos_da_requisicao[2] = strtok(NULL, " \t\n");

    printf("\nPrimeira info da requisição: %s\n", infos_da_requisicao[0]);
    printf("\nSegunda info da requisição:  %s\n", infos_da_requisicao[1]);
    printf("\nTerceira info da requisição: %s\n", infos_da_requisicao[2]);

    if (strcmp(infos_da_requisicao[0],"")){
      
      printf("\nDevolvendo Arquivo! Requisição: %s Arquivo: %s\n", infos_da_requisicao[0],infos_da_requisicao[1]);

      buscador_arquivos(infos_da_requisicao[1], socket);

    }

   printf("\n--Saiu Request Handler--\n");
   close(socket);
   return NULL;

}

void buscador_arquivos(char *nome_comp_do_arq, int socket){

   //Então temos o nome ou caminho do arquivo e o diretório no servidor que o arquivo está.
   //Agora vamos alocar uma variável para armazenar esse caminho completo, que é a união
   //do diretório do servidor e o caminho do arquivo

   //alocando uma variável dinâmicamente para armazenar o caminho total.
   //será utilizado para abrir o arquivo e enviar para o cliente
   char *caminho_total = (char * ) malloc((strlen(PATH) + strlen(nome_comp_do_arq)) * sizeof(char));
   char *tokens[2];
   long bytes_no_arquivo = 0;//para arquivo html
   char *buffer;
   int bytes_na_imagem;//para imagens
   char buffer_imagem[BUFFER_SIZE];
   FILE *file;//para html
   int file_img = 0;//para imagem
   
   strcpy(caminho_total, PATH);
   strcat(caminho_total, nome_comp_do_arq);

   printf("\nCaminho completo: %s\n", caminho_total);

   //Aqui estamos pegando o nome do arquivo e separnado pelo .
   //exemplo /index.html, "/index" será armazenado em tokens[0]
   tokens[0] = strtok(nome_comp_do_arq,".");
   // e "html" será armazenado em tokens[1]
   tokens[1] = strtok(NULL,".");

   printf("\nNome do Arquivo: %s", tokens[0]);
   printf("\nExtensao do arquivo: %s", tokens[1]);

   if(strcmp(tokens[1],"ico") != 0){ 
         if(strcmp(tokens[1],"html") == 0){

            file = fopen(caminho_total, "r");

            //Verificando se o arquivo foi encontrado
            if(file != NULL){

               printf("\nArquivo encontrado!\n");

               //Encontra o final do arquivo, neste caso, o arquivo HTML
               fseek(file, 0, SEEK_END);

               //Armazena a quantidade de bytes do arquivo na variavel
               bytes_no_arquivo = ftell(file);
               fseek(file, 0, SEEK_SET);

               //Envia ao navegador uma resposta de sucesso, indicando que a busca pelo item foi
               //bem sucedida. Cod 200.
               send(socket, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nKeep-Alive: timeout=5\r\nConnection: keep-alive\r\n\r\n", 44, 0);

               //Agora vamos alocar o quantidade de bytes do arquivo na variavel buffer
               //para poder enviar ao socket.
               buffer = (char *) malloc(bytes_no_arquivo * sizeof(char));

               //Aqui vamos de fato ler o arquivo html para armazenar no buffer   
               fread(buffer, bytes_no_arquivo, 1, file);
               //Agora que temos a informação no buffer, vamos escreve-la no socket
               write(socket, buffer, bytes_no_arquivo);

               free(buffer);
               free(caminho_total);
               fclose(file);

            } else {
               //Se file for == NULL, o arquivo não foi encontrado.
               printf("\nErro ao abrir arquivo!");

            }

         //Tratamento de imagens para enviar ao servidor
         } else if((strcmp(tokens[1], "jpg") == 0) || (strcmp(tokens[1], "jpeg") == 0) || (strcmp(tokens[1], "png"))){

            file_img = open(caminho_total, O_RDONLY);

            if(file_img > 0){

               printf("\nArquivo encontrado!\n");

               

               //Nota: Pesquisar o que são os parametros numéricos nessa função, não tenho certeza do que são
               //Informando que a requisição foi estabelecida com sucesso
               //Muito importante dar atenção ao tipo de resposta, exemplo: image/jpeg, senão o navegador não sabe interpretar o tipo de arquivo

               char *msg = (char * ) malloc(sizeof(char)* (strlen("HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n")+strlen("\r\nKeep-Alive: timeout=5\r\nConnection: keep-alive\r\n")));

               msg[0] = '\0';

               strcat(msg,"HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n");
               strcat(msg,"\r\nKeep-Alive: timeout=5\r\nConnection: keep-alive\r\n");


               printf("\nMesagem enviada ao cliente %s\n", msg);
               send(socket, msg, 45, 0);
               

               free(msg);

               //Aqui ele vai entrar nesse loop e vai ler até que a leitura retorne 0
               //o que significa que não tem mais nada para ler/chegou no final do arquivo de imagem
               while((bytes_na_imagem = read(file_img, buffer_imagem, BUFFER_SIZE)) > 0){ 
                  //Vai colocar no socket conforme o buffer for enchendo
                  //em partes até que a imagem inteira tenha sido carregada e enviada

                  write(socket, buffer_imagem, bytes_na_imagem);
                  //printf("\nl %d\n", bytes_na_imagem);
                  //printf("\nB %lu\n", strlen(buffer_imagem));
               }

               close(file_img);
               free(caminho_total); 

            } else {

               printf("\nArquivo de imagem não encontrado!\n");
               close(file_img);

            }

            
         }
      } else {

         printf("\nTentativa de buscar ico recusada!\n");

      }

   printf("\n--Saiu Buscador de Arquivos--\n");
}