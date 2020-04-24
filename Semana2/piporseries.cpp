/*
  Calcula el numero PI utilizando una serie "infinita"
  Debe recibir la cantidad de iteraciones como parametro
  pi = (-1)^i x 4/(2xi + 1)
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include "Buzon.h"
//#define KEY 0xA12347
using namespace std;
/*
   Realiza la acumulacion de terminos desde una posicion inicial hasta un termino final
*/




double calcularSumaParcialPi(Buzon & b, int tipo, int proceso, long inicial, long terminos ) {
 
   double casiPi = 0;
   double alterna = 4;
   long divisor = 0;
   long termino;

   for ( termino = inicial; termino < terminos; termino++ ) {
      divisor = termino + termino + 1;		// 2 x termino + 1
      casiPi += alterna/divisor;		// 4 / (2xi + 1)
      alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
   }
	//cout<<"Voy a enviar "<<casiPi<<endl;
   b.Enviar(casiPi,tipo);			// Guarda el resultado en el vector y finaliza

   exit( 0 );
}


int main( int argc, char ** argv ) {
   Buzon b;
   int st;
   int contador = 0;
   pid_t procesoCreador = getpid();
   //cout<<"proceso creador: "<<procesoCreador<<endl;
   long terminos, inicio, fin;
   int proceso;
   int pid;
   int tipo = 1314;
   double pi[100] = {0};
   double * piCompartido = &pi[0];
   double temp = 0;
   terminos = 1000000;
   if ( argc > 1 ) {
      terminos = atol( argv[ 1 ] );
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      inicio = proceso * terminos/10;
      fin = (proceso + 1) * terminos/10;
      pid = fork();
	  pid_t temp = getpid();
	  cout<<"Estoy en el proceso: "<<temp<<endl;
	  if( temp != procesoCreador){
		if (!pid ) {
            calcularSumaParcialPi(b, tipo, proceso, inicio, fin); //aqui no es
		} 
		else {
         //printf("Creating process %d: starting value %ld, finish at %ld\n", pid, inicio, fin );
		}
	  }else{
		  //aqui
		  //que no reciba todos mensajesç
		  //posible solucion utilizar un barrier
		 for (int p = 0; p < 10; p++ ) {
			  int status;
			  pid_t pid = wait( &status );
		 }
		  while((st = b.Recibir(piCompartido,contador, tipo)) > 0){
			  contador++;
		  }
	  }
      
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
   }
	for(int i = 0; i<10; ++i){
		cout<<"Vector"<<i<<": "<<piCompartido[i]<<endl;
	}


   for ( proceso = 1; proceso < 10; proceso++ ) {
      piCompartido[ 0 ] += piCompartido[ proceso ];
   }

   printf( "Valor calculado de Pi es \033[91m %g \033[0m con %ld terminos\n", piCompartido[ 0 ], terminos );


}