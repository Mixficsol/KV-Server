SOURCE = csapp.c echoserveri.c echoclient.c echo-server.cpp sbuf.c
CLIENT = csapp.o echoclient.o
SERVER = echoserveri.o echo-server.o csapp.o sbuf.o
all:
	g++ -c $(SOURCE)
	g++ -o client $(CLIENT) -lpthread -lhiredis
	g++ -o server $(SERVER) -lpthread -lhiredis
clear:
	rm *.o
