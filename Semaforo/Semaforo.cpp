#include "Semaforo.h"
#define KEY 0xA12345

Semaforo::Semaforo(int valorInicial){//CONSTRUCTOR
	procesoConstructor = getpid();
	int status;
	//parametros semget:
	//1- La clave con la que se accesa al semaforos
	//2- El numero de semaforos que queremos
	//3- Los permisos del semaforo(como si fuera una carpeta), en este caso lectura y escritura al propietario
	//y que los semaforos se creen al llamar a la funcion semget
	
	//es importante el 0 antes del 600 ya que el compilador lo tomara como octal y asignara bien los permisos
	id = semget(KEY,1,IPC_CREAT|0600);//CREA SEMÁFORO
	
	if(id == -1){//Si hay error de construccion, retorna -1 el semget
		perror("Error en construccion");
		exit(1);//SALIDA
	}
	
	//id es el identificador del array de semaforos que devuelve el semget al momento de crearlos(todos los semaforos de ese array van a tener el mismo id)
	
	semun u;//declara el semun que es el estado del semaforo(puede tomar varios valores de distinto tipo, pero aqui es un int)
    u.val = valorInicial;//le asigna el valor inicial al estado del semaforo, que se le pasa por parametro
	
	//parametros semctl:
	//1- es el id del array de semaforos que se crearon(siempre se crea un array de semaforos)
	//2- la posicion del semaforo que queremos inicializar dentro del array, como solo hay 1 entonces es la pos 0
	//3- indica que queremos hacer con el semaforo, en este caso es inicializar(SETVAL)
	//4- Indica el estado del semaforo(rojo,verde)
	//en el caso del semun, se va a convertir en un int que se le pasa por parametro en el main(que es un 0)
	status = semctl(id,0, SETVAL, u);

	if(status == -1){//Si hay error de inicializacion, semctl devuelve -1
		perror("Error en la inicialización");
		exit(1);//SALIDA.
	}
}

Semaforo::~Semaforo(){
	int resultado;
	//parametros semctl:
	//1- identificador de el grupo de semaforos
	//2- la posicion de el semaforo del array (que este inicializado) que queremos borrar
	//3- comando para el borrado de el set de semaforos
	pid_t proceso = getpid();
	if(proceso == procesoConstructor){
		resultado = semctl(id,0,IPC_RMID);
		if(-1 == resultado){
			perror("Error de destructor");
			exit(1);
		}
	}
	
}

void Semaforo::Wait(){
	//sembuf atributos:
	//1- es la posicion del semaforo en el array con el que queremos trabajar(como solo hay 1, la posicion siempre es 0)
	//2- es la operacion a realizar en el semaforo, como es negativo, se le resta 1 al contador del semaforo
	//3- son unas banderas que dicen como reacionara la operacion en caso de encontrarse un error
	struct sembuf vectorOperacionesW[]={0,-1,0};
	semop(id,vectorOperacionesW,1);
}

void Semaforo::Signal(){
	//sembuf atributos:
	//1- es la posicion del semaforo en el array con el que queremos trabajar(como solo hay 1, la posicion siempre es 0)
	//2- es la operacion a realizar en el semaforo, como es positivo, se le suma 1 al contador del semaforo
	//3- son unas banderas que dicen como reacionara la operacion en caso de encontrarse un error
	
	struct sembuf vectorOperacionesS[]={0,1,0};
	semop(id,vectorOperacionesS,1);
}

