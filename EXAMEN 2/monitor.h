
#include <mutex>
#include <iostream>
#include <semaphore.h>
using namespace std;

class Monitor {
private:
	sem_t sem;
	int carros;
	int carrosLlegados;
	int calles;
	bool controlador;
public:
  void Controlador(){
		int i = 0;
		cout<<"SOY CONTROLADOR, ESTOY ESPERANDO CARROS"<<endl;
		while( i < carros){
			
		}
  }
  void Carro(){
	  cout<<"SOY UN CARRO"<<endl;
	  llegoCarro();
	  
  }
  void llegoCarro(){
	  sem_wait(&sem);
	  cout<<"SEÑAL DE QUE LLEGO CARRO ENVIADA"<<endl;
	  this->carros++;
	  sem_post(&sem);
	  
  }
  void soyControlador(){
	  sem_wait(&sem);
	  this->controlador = true;
	  sem_post(&sem);
  }
  bool getControlador(){
	  return controlador;
  }
  Monitor(int K){
	  carros = K;
	  sem_init(&sem,1, 1);
	  controlador = false;
	  
  }
  void setCalles(int calles){
	  this->calles = calles;
	  
  }




};