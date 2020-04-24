/*
 *  Esta clase encapsula los llamados al sistema en UNIX, para intercambio de mensajes por medio de buzones
 *  Se provee facilidades para el envio y recepcion de mensajes de cualquier tipo definido por el usuario
 *
 *  Autor: CI0117 Programacion Paralela y Concurrente
 *  Fecha: 2020-i
 *
 *  Ejemplo de un mensaje que contiene etiquetas HTML y su cantidad de apariciones:
 *
 *  struct msgbuf {
 *     long mtype;	// Tipo del mensaje a enviar, tiene que estar de primero en la estructura
 *			// Esta variable, para enviar, debe ser > 0
 *     int veces;	// Cantidad de veces que aparece la etiqueta HTML
 *     char etiqueta[ 32];	// Etiqueta HTML
 *  };
 */

#define KEY 0xB84060	// Valor de la llave del recurso
#include <sys/types.h>
#include <unistd.h>


class Buzon{
	struct msgbuf{
		  long mtype;
		  int veces;
		  int size;
		  double suma; //Aqui se va a guardar la suma total de cada proceso antes de enviarla
		  char etiqueta[ 32 ];
	}mensaje;
   public:
	
	  
      Buzon();
      ~Buzon();
      int Enviar( const char *,int, long);	// Envia la tira de caracteres como un mensaje tipo
      int Enviar( double, long);
      int Recibir( char *,int*, long );   // len es el tamaño máximo que soporte la variable mensaje
      int Recibir( double*,int, long );   // mensaje puede ser de tipo msgbuf
	  pid_t procesoCreador;
   private:
      int id;		// Identificador del buzon

};