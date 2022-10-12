SOURCE = csapp.c echoservert.c echoclient.c echo.cpp sbuf.c
CLIENT = csapp.o echoclient.o
SERVER = echoservert.o echo.o csapp.o sbuf.o
all:
	g++ -c $(SOURCE)
	g++ -o client $(CLIENT) -lpthread
	g++ -o server $(SERVER) -lpthread
clear:
	rm *.o
