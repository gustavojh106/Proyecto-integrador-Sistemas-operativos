pruebaSemaforo: Semaforo.o pruebaSemaforo.o
	g++ -Wall -g -o pruebaSemaforo Semaforo.o pruebaSemaforo.o
Semaforo.o: Semaforo.cpp Semaforo.h
	g++ -c Semaforo.cpp
pruebaSemaforo.o: pruebaSemaforo.cpp Semaforo.h
	g++ -c pruebaSemaforo.cpp
.PHONY: clean
clean:
	rm -rf *.o