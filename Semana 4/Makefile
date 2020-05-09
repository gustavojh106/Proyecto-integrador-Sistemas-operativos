cliente: MirrorClient.o Socket.o
	g++ MirrorClient.o Socket.o -o cliente
server: MirrorServer.o Socket.o
	g++ MirrorServer.o Socket.o -o server
MirrorClient.o: MirrorClient.cpp Socket.h
	g++ -c MirrorClient.cpp
MirrorServer.o: MirrorServer.cpp Socket.h
	g++ -c MirrorServer.cpp
Socket.o: Socket.cpp Socket.h
	g++ -c Socket.cpp
.PHONY: clean
clean: