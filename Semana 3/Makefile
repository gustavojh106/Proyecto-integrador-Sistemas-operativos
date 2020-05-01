prueba6: ipv6.o Socket.o
	g++ ipv6.o Socket.o -o prueba6
prueba4: ipv4.o Socket.o
	g++ ipv4.o Socket.o -o prueba4
ipv6.o:	ipv6.cpp Socket.h
	g++ -c ipv6.cpp
ipv4.o: ipv4.cpp Socket.h
	g++ -c ipv4.cpp
Socket.o: Socket.cpp Socket.h
	g++ -c Socket.cpp
.PHONY: clean
clean: