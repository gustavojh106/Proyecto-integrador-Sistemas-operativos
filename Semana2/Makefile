enviar: enviarConClases.o Buzon.o
	g++ enviarConClases.o Buzon.o -o enviar
recibir: recibirConClases.o Buzon.o
	g++ recibirConClases.o Buzon.o -o recibir
piporseries: piporseries.o Buzon.o
	g++ piporseries.o Buzon.o -o piporseries
piporseriesMC: piporseriesMC.o
	g++ piporseriesMC.o -o piporseriesMC
enviarConclases.o: enviarConClases.cpp Buzon.h
	g++ -c enviarConClases.cpp
recibirConClases.o: recibirConClases.cpp Buzon.h
	g++ -c recibirConClases.cpp
piporseries.o: piporseries.cpp Buzon.h
	g++ -c piporseries.cpp
Buzon.o: Buzon.cpp Buzon.h
	g++ -c Buzon.cpp
piporseriesMC.o: piporseriesMC.cpp
	g++ -c piporseriesMC.cpp
.PHONY: clean
clean:
	rm -rf *.o
