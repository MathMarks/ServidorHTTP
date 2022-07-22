

#ifndef HTTPRequest_h
#define HTTPRequest_h

#include <stdio.h>
#include "C_fila.h"

//A ideia aqui é criar a estrutura, uma struct, que será responsável por carregar todas as 
//diferentes partes do cabeçalho request

//Como vamos implementar apenas GET, não será necessário uma estrutura para diferentes métodos.


//Essa função irá receber o pedido do cliente(Navegador) e irá tratar os diferentes pedidos que ele possa fazer.
//Para começar, vamos lidar apenas com html e jpg

void * buscador_arquivos(void * reqv);
void * request_handler(void * socket);
void * manipula_fila_request(void * arg);








#endif // !HTTPRequest_h


