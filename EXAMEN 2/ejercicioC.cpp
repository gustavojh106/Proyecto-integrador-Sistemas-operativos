
#include "monitor.h"
#include <pthread.h>
#define K 5

Monitor monitor(5);
typedef struct{
	int asd;

}datos_t;

void correr(void * ads){
	srand(pthread_self());
	int num = rand();
	if(!(monitor.getControlador())){
		monitor.soyControlador(); //CONVIERTE LA VARIABLE CONTROLADOR EN TRUE, ENTONCES SOLO HAY 1 CONTROLADOR
		monitor.Controlador();
	}
	else{
		monitor.Carro();
	}
}




int main(){
	int cantCalles = 0;
	cout<<"INDIQUE LA CANTIDAD DE CALLES QUE QUIERE: ";
	cin>>cantCalles;
	datos_t datos;
	
	
	pthread_t* carros = (pthread_t*)malloc((K+1)* sizeof(pthread_t));
	for(int i = 0; i< (K+1); ++i){
		pthread_create(&carros[i], NULL, correr, &datos);
	}

	for(int i = 0; i< (K+1); ++i){
		pthread_join(carros[i], NULL);
	}
}