SOURCE = csapp.c echoservert.c echoclient.c echo.cpp sbuf.c
CLIENT = csapp.o echoclient.o
SERVER = echoservert.o echo.o csapp.o sbuf.o
all:
	g++ -c $(SOURCE)
	g++ -o client $(CLIENT) -lpthread -lhiredis
	g++ -o server $(SERVER) -lpthread -lhiredis
clear:
	rm *.o
