#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include <semaphore.h>
#include <mutex>
#include <iostream>
using namespace std;

#define autosRecodo 7

void carroCalleA(int);
void carroCalleB(int);
struct Compartidos {//ESTRUCTURA PARA GUARDAR LOS DATOS COMPARTIDOS
		sem_t calleA;
		sem_t calleB;
		
		sem_t inX;
		sem_t inY;
		
		// mutex mtxX;
		// mutex mtxY;
	    mutex mtx;
};
	typedef struct Compartidos Compartir; //SE INICIALIZA LA ESTRUCTURA PARA DATOS COMPARTIDOS
	Compartir * pComp; //EL PUNTERO QUE SE VA A COMPARTIR CON "SHMGET"

	int id = 0; //EL IDENTIFICADOR DE LA MEMORIA COMPARTIDA
	
	
	
int main(){
		int i = 0;
		int carrosTotales;
		cout<<"INDIQUE LA CANTIDAD DE CARROS TOTALES: ";
		cin>>carrosTotales;
		
		id = shmget( 0xB84060, sizeof( Compartir ), 0600 | IPC_CREAT );
        if ( -1 == id ) {//SI HUBO UN ERROR AL CREAR LA SECCION COMPARTIDA
           perror("ERROR AL CREAR LA SECCION DE MEMORIA COMPARTIDA\n");
           exit( 0 );
        }
        pComp = ( Compartir * ) shmat( id, NULL, 0 ); 
		sem_init(&(pComp->calleA), 1, 7);
		sem_init(&(pComp->calleB), 1, 7);
		sem_init(&(pComp->inX),1, 1);
		sem_init(&(pComp->inY),1, 1);
		
		
		
		srand( time( NULL ) );
		cout<<"CARROS TOTALES: "<<carrosTotales<<endl;
		
		while( i < carrosTotales ) {
			int aleatorio = rand();
			cout<<"RAND: "<<aleatorio<<endl;
			if ( ! fork() ) { // SI ES EL PROCESO HIJO ENTONCES:
				if ( aleatorio % 2 ){
					//pComp->mtx.lock();
					cout<<"UN CARRO EN LA CALLE A"<<endl;
					//pComp->mtx.unlock();
					carroCalleA(i);
					//exit(1);
					break;
				} 
				else{ 
					//pComp->mtx.lock();
					cout<<"UN CARRO EN LA CALLE B"<<endl;
					//pComp->mtx.unlock();
					carroCalleB(i);
					//exit(1);
					break;
				}
			}
			i++;
		}
	
	
		//COMIENZA EL "JOIN" DONDE SE ASEGURARÁ QUE TODOS LOS HILOS LLEGUEN A ESTE PUNTO
	//ES COMO UN BARRIER ANTES DE DESTRUIR LOS RECURSOS

		for (int k = 0; k < carrosTotales; k++){
		  int status;
		  pid_t pid = wait( &status );
		}


        printf( "Destruyendo los recursos de memoria compartida\n");
		//SE DESTRUYE LA MEMORIA COMPARTIDA
        shmdt( pComp );
        shmctl( id, IPC_RMID, NULL );
	
}


void carroCalleA(int i){
	
	sem_wait(&(pComp->calleA)); //ANTES DE ENTRAR AL RECODO, VERIFICA SI HAY 7 ADENTRO
	cout<<"CARRO: "<<i<<" ";
	cout<<"HAY CAMPO EN EL RECODO A, PASO "<<endl;

	//ENTRO A LA INTERSECCION Y
	sem_wait(&(pComp->inX));
		cout<<"CARRO: "<<i<<" PASANDO LA INTERSECCION X (CARRO A)"<<endl;
		sleep(1); //TARDAN 1 SEGUNDO CRUZANDO LA INTERSECCION
	sem_post(&(pComp->inX));
	
	//EN ESTE PUNTO ESTÁN EN EL RECODO, ESPERANDO CRUZAR LA SIGUIENTE INTERSECCION
	cout<<"CARRO: "<<i<<" ";
	cout<<"ESTOY EN EL RECODO DE LA CALLE B"<<endl;
	//
		//TIEMPO EN EL RECODO ESPERANDO PASAR LA SEGUNDA INTERSECCION
	//
	cout<<"CARRO: "<<i<<" ";
	cout<<"ESTOY EN CALLE A ESPERANDO PARA PASAR LA INTERSECCION Y"<<endl;
	sem_wait(&(pComp->inY));
		//CRUZA LA INTERSECCION Y LIBERA UN ESPACIO EN EL RECODO
		cout<<"CARRO: "<<i<<" ";
		cout<<"CARRO A CRUZO INTERSECCION Y"<<endl;
		sem_post(&(pComp->calleA)); //SIGNAL A LOS CARROS DE LA CALLE B QUE NO HAN ENTRADO EN EL RECODO
		sleep(1);
	sem_post(&(pComp->inY));
	
	
	
	exit(1);
}

void carroCalleB(int i){
	
	sem_wait(&(pComp->calleB)); //ANTES DE ENTRAR AL RECODO, VERIFICA SI HAY 7 ADENTRO
	cout<<"CARRO: "<<i<<" ";
	cout<<"HAY CAMPO EN EL RECODO B, PASO "<<endl;

	//ENTRO A LA INTERSECCION Y
	sem_wait(&(pComp->inY));
	cout<<"CARRO: "<<i<<" PASANDO LA INTERSECCION Y (CARRO B)"<<endl;
	sleep(1); //TARDAN 1 SEGUNDO CRUZANDO LA INTERSECCION
	sem_post(&(pComp->inY));
	
	//EN ESTE PUNTO ESTÁN EN EL RECODO, ESPERANDO CRUZAR LA SIGUIENTE INTERSECCION
	cout<<"CARRO: "<<i<<" ";
	cout<<"ESTOY EN EL RECODO DE LA CALLE B"<<endl;
	//
		//TIEMPO EN EL RECODO ESPERANDO PASAR LA SEGUNDA INTERSECCION
	//
	cout<<"CARRO: "<<i<<" ";
	cout<<"ESTOY EN CALLE B ESPERANDO PARA PASAR LA INTERSECCION X"<<endl;
	sem_wait(&(pComp->inX));
		//CRUZA LA INTERSECCION Y LIBERA UN ESPACIO EN EL RECODO
		cout<<"CARRO: "<<i<<" ";
		cout<<"CARRO B CRUZO INTERSECCION X"<<endl;
		sem_post(&(pComp->calleB)); //SIGNAL A LOS CARROS DE LA CALLE B QUE NO HAN ENTRADO EN EL RECODO
		sleep(1);
	sem_post(&(pComp->inX));
	

	exit(1);
}












