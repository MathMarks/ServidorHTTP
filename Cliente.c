#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define PORT_NUMBER 80 //Porta utilizada
#define BUFFER_SIZE 4000 //Tamanho do buffer

int main(int argc, char **argv){

    int sockfd, n;
    int sendbytes;
    struct sockaddr_in servaddr;
    char sendline[BUFFER_SIZE];
    char recvline[BUFFER_SIZE];

    if (argc != 2)
    {
        perror("\nerro: Método de uso inválido\nEspera-se 2 argumentos Ex:GET /\n");
        return 1;
    }
    
    //Criando novo socket
    //A partir do socket que será possível ler dados e enviar dados 
    //através da conexão com o servidor
    //AF_INET - "Address Family - Internet" - Significa que é um socket para internet por assim dizer
    //SOCK_STREAM - Essa variável sinaliza que vamos enviar um fluxo de bytes, e não datagramas.
    // 0 - Protocolo TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Checagem para garantir que o socket foi criado corretamente 
    if (sockfd < 0)
    {
        printf("\nErro ao criar o socket no cliente! - Linha %d\n", __LINE__);
        return 1;

    }
    
    //Zerando a variável declarada
    bzero(&servaddr, sizeof(servaddr));

    //
    servaddr.sin_family = AF_INET;
    //"htons - host to network, short" - Garente que, mesmo que cliente e servidor usem diferentes ordens de dados
    // eles ainda possam se comunicar, então se um usa byte e outro bit, eles vão conseguir se comunicar
    servaddr.sin_port = htons(PORT_NUMBER);

    //converte a string que passamos como argumento ao iniciar o cliente para para uma forma "binária"
    // "127.0.0.1 - [127,0,0,1]"
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr)<=0){

        printf("\nErro durante a conversão do endereço ip fornecido de string para binário - linha %d\n", __LINE__);

        return 1;

    }

    //Agora vamos utilizar a função connect para conectar a aquele ip que fornecemos ao iniciar o cliente
    //Realiza a conexão e testa se não teve nenhum erro.
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
        printf("\nFalha ao ao se conectar ao servidor - linha %d\n", __LINE__);

        return 1;
    }

    //Agora com a conexão estabelecida, podemos enviar alguma requisição ao servidor
    //A requisição abaixo é um teste e muito simples, basicamente estamos pedindo
    //pela rootpage "/" de qualquer servidor(ip) que selecionamos
    //O \r\n\r\n significa que aquele é o final da mensagem de requisição
    sprintf(sendline, "GET / HTTP/1.0\r\n\r\n");
    //Essa mensagem acima é o próximo passo, vamos criar métodos que vão possibilitar criar
    //mensagens mais complexas 
    sendbytes = strlen(sendline);

    //Se o retorno da função write for diferente do tamanho da mensagem a ser enviada
    //teve algum problema durante a escrita no socket e esse erro é disparado.
    //Então, se não tiver nenhum problema, a função socket irá enviar todos os characteres
    //pela conexão até o servidor, e se tudo ocorrer conforme deveria, vamos ter uma resposta 
    // do servidor e poderemos fazer o que quisermos com a resposta.
    if(write(sockfd, sendline, sendbytes) != sendbytes){

        printf("\nErro na escrita da mensagem no socket, linha %d\n", __LINE__);

        return 1;

    }

    //Usando a função read, vamos ler do socket e vamos pegar esse dado e imprimir no terminal
    //mas podemos a partir daqui fazer o que quisermos com esse dado, salvar em um arquivo etc.
    while ((n = read(sockfd, recvline, BUFFER_SIZE-1)) > 0)
    {
        printf("%s", recvline);
        //Essa função garante que a cada vez a variavel recvline está em null
        memset(recvline, 0, BUFFER_SIZE);
    }

    if(n < 0){

        printf("\nHouve algum erro durante a leitura dos dados no socket, linha %d\n", __LINE__);

    }

exit(0);

}