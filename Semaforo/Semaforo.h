/*
   C++ class to encapsulate Unix semaphore intrinsic structures and
   system calls
*/
#include <sys/types.h>//
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <iostream>
#include <string>

#define KEY 0xA12345	// Valor de la llave del recurso

using namespace std;

class Semaforo {
   public:
      Semaforo( int ValorInicial = 0 );
      ~Semaforo();
      pid_t procesoConstructor;
      void Signal();	// Puede llamarse V
      void Wait();	// Puede llamarse P
      union semun{//STRUCT PARA SEMCTL
        	int val;
		struct semid_ds *buf;
		ushort *array;
        struct seminfo *_buf;
	  };
      int id;		// Identificador del semaforo
};
