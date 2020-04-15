piporseries: piporseries.o
	g++ -o piporseries piporseries.o
piporseries.o: piporseries.cpp
	g++ -c piporseries.cpp