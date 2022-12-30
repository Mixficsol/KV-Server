SOURCE := $(wildcard src/*.cc)
CLIENT := csapp.o echoclient.o kv_encode.o
SERVER := csapp.o echoserveri.o kv_encode.o kv_io.o kv_cluster_epoll.o kv_coon.o storage_engine.o

CXXFLAGS += -g
INCLUDE_PATH = -I ./include \

all:
	g++ -c $(CXXFLAGS) $(INCLUDE_PATH) $(SOURCE)
	g++ -o client $(CXXFLAGS) $(CLIENT) -lpthread -lleveldb -lboost_system -lboost_filesystem -lglog
	g++ -o server $(CXXFLAGS) $(SERVER) -lpthread -lleveldb -lboost_system -lboost_filesystem -lglog

clear:
	rm *.o
	rm -r client server
