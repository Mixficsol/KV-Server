SOURCE := $(wildcard src/*.c src/*.cpp)
CLIENT := csapp.o echoclient.o
SERVER := echoserveri.o echo.o csapp.o sbuf.o

all:
	g++ -c $(SOURCE)
	g++ -o client $(CLIENT) -lpthread -lleveldb -lboost_system -lboost_filesystem 
	g++ -o server $(SERVER) -lpthread -lleveldb -lboost_system -lboost_filesystem

clear:
	rm *.o
