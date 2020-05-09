/*
 *  Establece la definicion de la clase Socket para efectuar la comunicacion
 *  de procesos que no comparten memoria, utilizando un esquema de memoria
 *  distribuida.  El desarrollo de la clase se hara en dos etapas, primero
 *  los metodos necesarios para los clientes, en la otra etapa los otros
 *  metodos para los servidores
 */

#ifndef Socket_h
#define Socket_h
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>

class Socket{

    public:
        Socket( char, bool = false );
        Socket();
		Socket(int);
        ~Socket();
        int Connect( const char *, int ); //para ipv4
        int Connect( const char *, char * ); // para ipv6
        void Close();
        int Read( const char* , int ); // lee los datos del servidor y los guarda
        int Write( const char* ); //pide una solicitud al servidor para poder leer
		int Listen( int );
        int Bind( int );
        Socket * Accept();
        int Shutdown( int );
        void SetIDSocket( int );
		struct sockaddr_in address;
		
		
    private:

        int idSocket;
        bool ipv6;
};

#endif