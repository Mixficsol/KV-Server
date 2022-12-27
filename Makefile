SOURCE := $(wildcard src/*.cc)
CLIENT := csapp.o echoclient.o kv_encode.o
SERVER := csapp.o echoserveri.o echo.o kv_encode.o kv_io.o kv_cluster_epoll.o kv_leveldb.o kv_coon.o

all:
	g++ -c $(SOURCE)
	g++ -o client $(CLIENT) -lpthread -lleveldb -lboost_system -lboost_filesystem 
	g++ -o server $(SERVER) -lpthread -lleveldb -lboost_system -lboost_filesystem

clear:
	rm *.o
	rm -r client server
