#include "Buzon.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;

//typedef msgbuf mensaje;
Buzon:: Buzon(){
	id = msgget(KEY,IPC_CREAT| 0600);
	if(-1 == id){
			perror("Error al crear la cola de mensajes");
			exit(1);
	}
	
}
Buzon:: ~Buzon(){
	int st;
	st = msgctl( id, IPC_RMID, NULL );
	if(-1 == st){
	  perror( "Error al borrar el buzon" );
	  exit( 2 );
	}		
}
int Buzon::Enviar(const char * msj, int i, long tipo){
	int resp;
	int size = strlen(msj);
	mensaje.mtype = tipo;
	mensaje.veces = i;
	mensaje.size = size;
	strncpy( mensaje.etiqueta, msj,  size ); //copia el msj que recibe como parametro a mensaje.etiqueta
	resp = msgsnd( id, (const void *) & mensaje, sizeof( mensaje ), IPC_NOWAIT );
	if(-1 == resp){
		printf("No se pudo enviar el mensaje /n");
		//exit(1);
	}
	return resp;
}

int Buzon::Enviar(double suma, long tipo){ //PERFECTO
	int resp;
	mensaje.mtype = tipo;
	mensaje.suma = suma;
	resp = msgsnd( id, (const void *) & mensaje, sizeof( mensaje ), IPC_NOWAIT );
	if(-1 == resp){
		printf("No se pudo enviar el mensaje");
		//exit(1);
	}
	return resp;
}


//el char * m es donde se va a guardar la informacion que contiene el mensaje
//veces no importa
//tipo es el tipo de mensaje que va a recibir el metodo 
int Buzon::Recibir(char * m, int* i, long tipo){
	int st;
	//El proceso se queda esperando hasta que le envien otro mensaje, se elimine la cola o se interrumpa de alguna forma
	st = msgrcv( id, (void *) & mensaje, sizeof( mensaje ), tipo, IPC_NOWAIT );
	int size = mensaje.size;
	strncpy(m,mensaje.etiqueta,size);
	
	
	// for(int i = 0; i < strlen(m); ++i){
		// if(m[i] == 'N'){
			// for(int k = 0; k < size; ++k){
				
			// }
		// }
	// }
	//strncpy( m, mensaje.etiqueta,  size ); //copia el msj que recibe como parametro a mensaje.etiqueta
    //printf( "Label: %s, veces %d, status %d \n", mensaje.etiqueta, mensaje.veces, st );
	
	return st;
}

int Buzon::Recibir(double * vector, int pos ,long tipo){
	int st;
	//El proceso se queda esperando hasta que le envien otro mensaje, se elimine la cola o se interrumpa de alguna forma
	st = msgrcv( id, (void *) & mensaje, sizeof( mensaje ), tipo, IPC_NOWAIT );
	double d = mensaje.suma;
	//printf( "Recibi: %5.2f \n", d);
	vector[pos] = mensaje.suma;
	//cout<<"Guarde "<<d<<" en la pos "<<pos<<endl;
	return st;
}



