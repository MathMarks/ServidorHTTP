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
#include "C_fila.h"
#include "HTTPRequest.h"
#include <time.h>
#include <sys/time.h>


char PATH[100];

void atualiza_path(){

   if (getcwd(PATH, sizeof(PATH)) != NULL) {

      //Mostra no console qual o diretório atual do servidor, apenas para debug. 
      printf("\n\nDiretório atual: %s\n\n\n", PATH);

   } else {
      perror("\nErro ao encontrar o diretório."); 
      
   }

}

void * request_handler(void * c_socket){
   //pthread_t thread = (pthread_t) malloc(sizeof(pthread_t));
   //pthread_create(&thread, NULL, manipula_fila_request,NULL);

   //Atualiza a variável PATH para o atual PATH do usuário.  
   atualiza_path();
   int socket = * ((int*)c_socket); 
   free(c_socket);
   
   char buffer[BUFFER_SIZE];
   char *infos_da_requisicao[4];
   int count = 0;
   
   while(recv(socket, buffer, BUFFER_SIZE,0) > 0){ 
      //printf("\n------Dentro do request_handler----\nRequest n°%d\n", count+1); 
      //read(socket, buffer, BUFFER_SIZE);
      //printf("\nInicio Requisição Cliente\n");
      printf("%s\n", buffer);
      //printf("\nFinal Requisição Cliente\n");

      //Extrair o nome do arquivo requisitado pelo cliente de dentro da variável buffer.
      

      //printf("%s\n", request); //Imprime toda a requisição do cliente e informações sobre ele

      //Captura o metodo, que sempre será o GET no caso dessa implementação
      infos_da_requisicao[0] = strtok(buffer, " \t\n");
      //Captura o arquivo que o cliente está solicitando, html ou jpg até o momento
      infos_da_requisicao[1] = strtok(NULL, " \t");
      //captura a versão do HTTP em uso
      infos_da_requisicao[2] = strtok(NULL, " \t\n");
      strtok(NULL, " \t\n");//Usados para percorrer a string dentro do buffer
      strtok(NULL, " \t\n");//Usados para percorrer a string dentro do buffer
      strtok(NULL, " \t\n");//Usados para percorrer a string dentro do buffer
      //Captura o cliente
      infos_da_requisicao[3] = strtok(NULL, " \t\n");


      printf("\nProtocolo da requisição: %s", infos_da_requisicao[2]);
      printf("\nTipo de requisição     : %s", infos_da_requisicao[0]);
      printf("\nArquivo da requisição  : %s", infos_da_requisicao[1]);
      printf("\nCliente                : %s", infos_da_requisicao[3]);

      if (strlen(infos_da_requisicao[1])>0){
         //printf("\nDevolvendo Arquivo! Requisição: %s Arquivo: %s\n", infos_da_requisicao[0],infos_da_requisicao[1]);
         insere_fila_request(&socket, infos_da_requisicao[1]);
         //printf("\nInseriu Fila\n");
         //pthread_create(&thread, NULL, buscador_arquivos, cria_request(infos_da_requisicao[1], &socket));
         //buscador_arquivos(cria_request(infos_da_requisicao[1], &socket));
      } 
   count++;
   }


      //printf("\n--Saiu Request Handler--\n");
      //sclose(socket);
   close(socket);
   //free(socket);
return NULL;
} 

void * buscador_arquivos(void * reqv){

   requestInfo * req = (requestInfo*)reqv;

   char *nome_comp_do_arq = req->buffer;
   int socket = *((int*)req->socket);
   free(req);
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
   FILE *file_img_size;
   int file_img = 0;//para imagem
   struct timeval tmp_ini, tmp_final; 
   
   strcpy(caminho_total, PATH);
   strcat(caminho_total, nome_comp_do_arq);

   //printf("\nCaminho completo: %s\n", caminho_total);

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

               //Agora vamos alocar o quantidade de bytes do arquivo na variavel buffer
               //para poder enviar ao socket.
               buffer = (char *) malloc(bytes_no_arquivo * sizeof(char));
         

               //Aqui vamos de fato ler o arquivo html para armazenar no buffer   
               fread(buffer, bytes_no_arquivo, 1, file);
               char tamanho_string[10];
               sprintf(tamanho_string,"%ld",bytes_no_arquivo);

               printf("\nTamanho do arquivo: %s", tamanho_string);
               //printf("\ntamanho real: %d",sprintf(tamanho_string,"%ld",bytes_no_arquivo));

               char *msg_html = (char *) malloc(strlen("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:")+sprintf(tamanho_string,"%ld",bytes_no_arquivo)+strlen("\r\nConnection:keep-alive\r\n\r\n"));
               
               strcpy(msg_html,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:");
               strcat(msg_html,tamanho_string);
               strcat(msg_html,"\r\nConnection:keep-alive\r\n\r\n");
               printf("\nResposta do Servidor ao Cliente:\n%s",msg_html);
               //Envia ao navegador uma resposta de sucesso, indicando que a busca pelo item foi
               //bem sucedida. Cod 200.
               send(socket,msg_html , strlen(msg_html), 0);

               gettimeofday(&tmp_ini, NULL);
               //Agora que temos a informação no buffer, vamos escreve-la no socket
               write(socket, buffer, bytes_no_arquivo);
               gettimeofday(&tmp_final, NULL);

               //printf("\nTempo m ini: %ld",tmp_ini.tv_usec);
               //printf("\nTempo m final: %ld",tmp_final.tv_usec);
               printf("\nTempo RTT: %ldms",(tmp_final.tv_usec - tmp_ini.tv_usec)*2);
                     
               fclose(file);

            } else {
               //Se file for == NULL, o arquivo não foi encontrado.
               printf("\nErro ao abrir arquivo!");

            }

         //Tratamento de imagens para enviar ao servidor
         } else if((strcmp(tokens[1], "jpg") == 0) || (strcmp(tokens[1], "jpeg") == 0) || (strcmp(tokens[1], "png"))){
            
            char tamanho_string_img[10];
            file_img_size = fopen(caminho_total, "r");
            fseek(file_img_size, 0, SEEK_END);

               //Armazena a quantidade de bytes do arquivo na variavel
            bytes_na_imagem =(int) ftell(file_img_size);
            fseek(file_img_size, 0, SEEK_SET);
            //Transformando o tipo da variavel, ela está em int e precisamos dela em string
            sprintf(tamanho_string_img,"%d",bytes_na_imagem);

            fclose(file_img_size);

            file_img = open(caminho_total, O_RDONLY);

            if(file_img > 0){

               printf("\nArquivo encontrado!\n");

               //Informando que a requisição foi estabelecida com sucesso

               
               //Muito importante dar atenção ao tipo de resposta, exemplo: image/jpeg, senão o navegador não sabe interpretar o tipo de arquivo
               //Modificar isso aqui depois para receber png tbm
               char *msg = (char * ) malloc(sizeof(char)* (strlen("HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length:")+sprintf(tamanho_string_img,"%d",bytes_na_imagem)+strlen("\r\nConnection: keep-alive\r\n\r\n")));

               msg[0] = '\0';

               strcat(msg,"HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length:");
               strcat(msg,tamanho_string_img);
               strcat(msg,"\r\nConnection: keep-alive\r\n\r\n");


               printf("\nMesagem enviada ao cliente %s", msg);
               send(socket, msg, strlen(msg), 0);

               free(msg);

               //Aqui ele vai entrar nesse loop e vai ler até que a leitura retorne 0
               //o que significa que não tem mais nada para ler/chegou no final do arquivo de imagem
               
               for(int i = 0;(bytes_na_imagem = read(file_img, buffer_imagem, BUFFER_SIZE)) > 0; i++){ 
                  //Vai colocar no socket conforme o buffer for enchendo
                  //em partes até que a imagem inteira tenha sido carregada e enviada
                  gettimeofday(&tmp_ini, NULL);
                  write(socket, buffer_imagem, bytes_na_imagem);
                  gettimeofday(&tmp_final, NULL);
                  printf("\nTempo RTT: %ldms para a %d° parte da imagem, Tamanho: %d",(tmp_final.tv_usec - tmp_ini.tv_usec)*2, i, bytes_na_imagem);

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

   //printf("\n--Saiu Buscador de Arquivos--\n");
   return NULL;
}