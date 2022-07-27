
all: run

run: main.o C_fila.o Servidor.o HTTPRequest.o
	gcc -o http main.o C_fila.o Servidor.o HTTPRequest.o

main.o: main.c C_fila.h Servidor.h HTTPRequest.h
	gcc -o main.o main.c -c -W -Wall  -pedantic

C_fila.o: C_fila.c C_fila.h
	gcc -o C_fila.o C_fila.c -c -W -Wall  -pedantic

Servidor.o: Servidor.c Servidor.h
	gcc -o Servidor.o Servidor.c -c -W -Wall  -pedantic

HTTPRequest.o: HTTPRequest.c HTTPRequest.h
	gcc -o HTTPRequest.o HTTPRequest.c -c -W -Wall  -pedantic

clean:
	rm -rf *.o *~ http
