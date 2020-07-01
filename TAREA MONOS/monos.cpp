#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
using namespace std;
mutex mtx;
typedef struct{
	int direccionCruzar; //LOS MONOS VAN A EMPEZAR CRUZANDO DE IZQ A DER
	int cantMonosCruzados;
	int contador;
	int monosCruzando;
	int K;
	int J;
	int N;
	int monosIZQ;
	int monosDER;
	int monosEsperando;
	int monosLiberar;
	int llegados;
	//pthread_mutex_t mutex;
	sem_t sem1;
	sem_t sem2;
	sem_t sem3;

}datos_t;


void cruzar(int miDireccion, datos_t * datos_compartidos, int pid){
	mtx.lock();
	datos_compartidos->cantMonosCruzados++;
	datos_compartidos->contador++;
	mtx.unlock();
	sleep(2);
	mtx.lock();
	cout<<"MONOS DER: "<<datos_compartidos->monosDER<< " MONOS IZQ: "<<datos_compartidos->monosIZQ<<endl;
	mtx.unlock();
	
	if(miDireccion == 0){
		mtx.lock();
		datos_compartidos->monosIZQ --;
		mtx.unlock();
		//sleep(4);
		mtx.lock();
		cout<<"EL MONO "<<pid <<" CRUZO DE DERECHA A IZQUIERDA"<<endl;
		mtx.unlock();
	}
	else{
		mtx.lock();
		datos_compartidos->monosDER --;
		mtx.unlock();
		//sleep(4);
		mtx.lock();
		cout<<"EL MONO "<<pid<<" CRUZO DE IZQUIERDA A DERECHA"<<endl;
		//cout<<"EL HILO "<<pid<<" DEJO EL CONTADOR EN: "<<datos_compartidos->contador<<" Y EL MONOS CRUZADOS EN: "<<datos_compartidos->cantMonosCruzados<<endl;
		mtx.unlock();
	}
	//EL ULTIMO MONO EN LLEGAR ES EL QUE AVISA A LOS DE LA DIRECCION CONTRARIA QUE PUEDEN PASAR
	if((datos_compartidos->contador) == (datos_compartidos->J) || (datos_compartidos->monosDER) == 0 || (datos_compartidos->monosIZQ) == 0){//CAMBIAMOS DE DIRECCION CUANDO YA PASARON J MONOS DE UNA MISMA DIRECCION O CUANDO YA NO QUEDEN MAS MONOS QUE VAN A UNA DIRECCION
		mtx.lock();
		//cout<<"EL MONO "<<pid<<" ES EL ULTIMO, ENTONCES VA A LIBERAR A LOS OTROS"<<endl;
		
		mtx.unlock();
		if((datos_compartidos->direccionCruzar) == 0){ //SE CAMBIA DE DIRECCION
			if(datos_compartidos->monosDER > 0){//SI YA NO QUEDAN MAS MONOS A LA DERECHA, NO CAMBIO DE DIRECCION, SIGO EN IZQ
				mtx.lock();
				datos_compartidos->direccionCruzar = 1;
				cout<<"EL MONO "<<pid<<" CAMBIO LA DIRECCION A LA DERECHA"<<endl;
				mtx.unlock();
			}
			
		}
		else{
			if(datos_compartidos->monosIZQ > 0){//SI YA NO QUEDAN MAS MONOS A LA IZQUIERDA, NO CAMBIO DE DIRECCION, SIGO EN DERECHA
				mtx.lock();
				datos_compartidos->direccionCruzar = 0;
				cout<<"EL MONO "<<pid<<" CAMBIO LA DIRECCION A IZQUIERDA"<<endl;
				mtx.unlock();
			}
		}
	
		
		//mtx.lock();
		//cout<<"LOS MONOS ESPERANDO: "<<datos_compartidos->monosEsperando<<endl;
		//cout<<"LOS MONOS A LIBERAR: "<<datos_compartidos->monosLiberar<<endl;
		
		//mtx.unlock();
		for(int i = 0; i<(datos_compartidos->monosEsperando); ++i){
			sem_post(&(datos_compartidos->sem3));
		}
		sleep(2);
		sem_init(&(datos_compartidos->sem3),0,0);//UNA VEZ LIBERADOS TODOS LOS MONOS, ESTE SEMAFORO SE PONE EN 0
		datos_compartidos->monosEsperando = 0;
		for(int i = 0; i<(datos_compartidos->monosLiberar);++i){
			sem_post(&(datos_compartidos->sem1));
		}
		//mtx.lock();
		//cout<<"YA LIBERE A TODOS LOS HILOS "<<endl;
		//mtx.unlock();
		
		datos_compartidos->monosLiberar = 0;
		datos_compartidos->contador = 0;


	}

}
void * correr(void * data){ //pasa un puntero a mi structura de datos compartida
	datos_t * datos_compartidos = (datos_t*)data;
	//int pid = pthread_self();
	//SE GENERAN LAS DIRECCIONES DE LOS MONOS ALEATORIAMENTE
	// mtx.lock();
	// cout<<"DATOS: "<<datos_compartidos->N<<endl;
	// cout<<"DATOS: "<<datos_compartidos->cantMonosCruzados<<endl;
	// mtx.unlock();
	int yaPase = 0;
	srand(pthread_self());
	int miDireccion = 1;
	int num = rand()%101; //SE GENERA UN ALEATORIO ENTRE 0 Y 100
	int pid = num;	
	mtx.lock();
	cout<<"HILO: "<<pid<<" ALEATORIO: "<<num<<endl;
	mtx.unlock();
	if(num < 50){
		miDireccion = 0;
		datos_compartidos->monosIZQ ++;
	}
	else{
		miDireccion = 1;
		datos_compartidos->monosDER ++;
	}

	//printf("HOLA\n");
	// mtx.lock();
	// cout<<"MONOS CRUZADOS: "<<datos_compartidos->cantMonosCruzados<<endl;
	// cout<<"MONOS TOTALES: "<<datos_compartidos->N<<endl;
	// mtx.unlock();
	
	while(((datos_compartidos->cantMonosCruzados) < (datos_compartidos->N)) && (yaPase == 0)){
		//mtx.lock();
		//cout<<"HILO: "<<pid<<" ENTRO AL WHILE GRANDE "<<endl;
		//mtx.unlock();
		if(miDireccion == (datos_compartidos->direccionCruzar)&& (yaPase == 0)){
			//mtx.lock();
			//cout<<"LA DIRECCION ACTUAL ES: "<<datos_compartidos->direccionCruzar<<endl;
			//cout<<"HILO: "<<pid<<" TIENE LA DIRECCION: "<<miDireccion<<endl;
			//mtx.unlock();
			if((datos_compartidos->contador) < (datos_compartidos->J)){
				mtx.lock();
				//cout<<"HILO: "<<pid<<" ENTRO AL SEGUNDO IF YA QUE CONTADOR: "<<datos_compartidos->contador<<" Y EL J: "<<datos_compartidos->J<<endl;
				mtx.unlock();
				mtx.lock();
				datos_compartidos->monosLiberar++;
				mtx.unlock();
				mtx.lock();
				cout<<"EL HILO "<<pid<<" ENTRO AL SEM1 A ESPERAR"<<endl;
				mtx.unlock();
				sem_wait(&(datos_compartidos->sem1));//CONTROLA QUE ENTREN J MONOS
				mtx.lock();
				//cout<<"EL HILO "<<pid<<" SUPERO EL WAIT DE SEM1"<<endl;
				mtx.unlock();
				if((datos_compartidos->contador) < (datos_compartidos->J)){
					mtx.lock();
					//cout<<"EL HILO "<<pid<<" ENTRO AL TERCER IF PORQUE CONTADOR: "<<datos_compartidos->contador<<" Y EL J "<<datos_compartidos->J<<endl;
					datos_compartidos->monosLiberar--;
					mtx.unlock();
					//sem 2 SEMAFORO QUE CONTROLA QUE PASEN DE K EN K MONOS
					cruzar(miDireccion, datos_compartidos, pid);
					//SIGNAL
					mtx.lock();
					//cout<<"YA CRUCE, ME SALGO"<<endl;
					cout<<"YA PASE, ME SALGO"<<endl;
					sem_post(&(datos_compartidos->sem1));
					//sleep(5);
					yaPase = 1;
					mtx.unlock();
					//pthread_exit(NULL); //UNA VEZ EL MONO CRUZO, SE SALE
				}
			}
		}
		else{
			mtx.lock();
			cout<<"HILO: "<<pid<<" ENTRO AL SEMAFORO 2 A ESPERAR"<<endl;
			datos_compartidos->monosEsperando++; //LA CANTIDAD DE MONOS ESPERANDO, AUMENTA
			mtx.unlock();
			sem_wait(&(datos_compartidos->sem3));//CONTROLA QUE LOS MONOS CON DIFERENTE DIRECCION ESPEREN HASTA QUE SE LES INDIQUE
		}
	}
	mtx.lock();
	datos_compartidos->llegados++;
	mtx.unlock();
	while(datos_compartidos->llegados < datos_compartidos->N){
		
	}
	
}

int main(int argc, char* argv[]){
	int miDireccion; //SE GENERA UNA DIRECCION ALEATORIA PARA ESTE MONO
	int pId = 0;
	datos_t datos_compartidos;
	//ASIGNAMOS LOS PARAMETROS

	datos_compartidos.N = atoi(argv[1]);
	datos_compartidos.K = atoi(argv[2]);
	datos_compartidos.J = atoi(argv[3]);
	datos_compartidos.direccionCruzar = 1;
	datos_compartidos.monosLiberar = 0;
	datos_compartidos.contador = 0;
	datos_compartidos.cantMonosCruzados = 0;
	datos_compartidos.monosEsperando = 0;
	datos_compartidos.monosDER = 0;
	datos_compartidos.monosIZQ = 0;
	datos_compartidos.llegados = 0;
	//SE IMPRIMEN LOS DATOS LEIDOS

	printf("N: %d\n",datos_compartidos.N);
	printf("K: %d\n",datos_compartidos.K);
	printf("J: %d\n",datos_compartidos.J);
	//INICIALIZA EL MUTEX
	//pthread_mutex_init( &datos_compartidos.mutex, NULL );
	sem_init(&(datos_compartidos.sem1),0,datos_compartidos.J);//SEMAFORO PARA CONTROLAR QUE ENTREN J MONOS DE UNA MISMA DIRECCION
	sem_init(&(datos_compartidos.sem2),0,datos_compartidos.K);//SEMAFORO PARA QUE DE LOS J MONOS QUE ENTRARON, PASEN DE K EN K
	sem_init(&(datos_compartidos.sem3),0,0);//SEMAFORO PARA HACER ESPERAR A LOS MONOS CON LA DIRECCION CONTRARIA A LA ACTUAL


	pthread_t* monos = (pthread_t*)malloc(datos_compartidos.N * sizeof(pthread_t));
	for(int i = 0; i< (datos_compartidos.N); ++i){
		pthread_create(&monos[i], NULL, correr, &datos_compartidos);
	}
	for(int i = 0; i< (datos_compartidos.N); ++i){
		pthread_join(monos[i], NULL);
	}
	return 0;
}
