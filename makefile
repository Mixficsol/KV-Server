SOURCE = csapp.c echoserveri.c echoclient.c echo.cpp sbuf.c
CLIENT = csapp.o echoclient.o
SERVER = echoserveri.o echo.o csapp.o sbuf.o
all:
	g++ -c $(SOURCE)
	g++ -o client $(CLIENT) -lpthread -lhiredis -lleveldb
	g++ -o server $(SERVER) -lpthread -lhiredis -lleveldb
clear:
	rm *.o
