#include "Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>
using namespace std;
#define SERVER_PORT 9876
/* 
   char tipo: el tipo de socket que quiere definir
      's' para "stream
      'd' para "datagram"
   bool ipv6: si queremos un socket para IPv6
 */
 
//parametros de la funcion socket:
//dominio: AF_INET o AF_INET6 (si es ipv6), indica si usa protocolos TCP/IP o UDP, o es comunicacion entre la misma maquina
//tipo: el tipo de socket (stream o datagrama
//protocolo: el protocolo que usara el socket, si se deja en 0 el sistema escoge
Socket::Socket( char tipo, bool ipv6 ){
	if(tipo == 's'){
		if(ipv6){
			idSocket = socket(AF_INET6, SOCK_STREAM,0);
		}
		else{
			idSocket = socket(AF_INET, SOCK_STREAM,0);
		}
	}
	if(tipo == 'd'){
		if(ipv6){
			idSocket = socket(AF_INET6,SOCK_DGRAM,0);
		}
		else{
			idSocket = socket(AF_INET,SOCK_DGRAM, 0);
		}
	}
	if(-1 == idSocket){
		perror("ERROR EN EL COSNTRUCTOR");
	}
}


Socket::~Socket(){
    Close();
}


void Socket::Close(){
	int resp = close(idSocket);
	if(-1 == resp){
		perror("ERROR AL CERRAR EL SOCKET");
		exit(1);
	}
}

/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   int port: ubicacion del proceso, por ejemplo 80
 */
int Socket::Connect( const char * hostip, int port ) { 
   int resp;
   struct sockaddr_in contenedor;
   
   contenedor.sin_family = AF_INET; //asigna el tipo (ipv4)
   inet_aton( hostip, & contenedor.sin_addr); //convierte la direccion ip a binario y lo guarda en el contenedor
   contenedor.sin_port = htons(port);
   
 
   resp = connect(idSocket,(const sockaddr*) & contenedor, sizeof(contenedor));
   if(-1 == resp){
	   perror("ERROR AL CONECTAR CON EL SERVIDOR MEDIANTE IPV4");
	   close(idSocket);
   }
   return resp;

}


/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   char * service: nombre del servicio que queremos acceder, por ejemplo "http"
 */
int Socket::Connect( const char *host, char *service ) {
	int resp;
	
	struct sockaddr_in6 contenedor;
	
	contenedor.sin6_family = AF_INET6;
	
	inet_pton(AF_INET6, host, &contenedor.sin6_addr);
	
	contenedor.sin6_port = htons(SERVER_PORT);
 
	/* Try to do TCP handshake with server */
	resp = connect(idSocket, (struct sockaddr*)& contenedor, sizeof(contenedor));
	if (-1 == resp) {
		perror("ERROR AL CONECTAR CON EL SERVIDOR MEDIANTE IPV6");
		close(idSocket);
	}
	return resp;

}


int Socket::Read( const char* text, int len ) {
	int resp;
	resp = read(idSocket,(void*)text, len);
	if(-1 == resp){
		printf("Error read");
		close(idSocket);
	}
   	return resp;
}

//pide una solicitud al servidor para que 
int Socket::Write( const char* text, int len ) { //escribir en la cola de solicitudes
	int resp;
	resp = write(idSocket,(const void*)text, len);
	if(-1 == resp){
		printf("Error write");
		close(idSocket);
	}
   	return resp;


}

