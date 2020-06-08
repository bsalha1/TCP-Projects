CC = g++
CFLAGS = -Wall -Werror -pedantic -Wconversion -pthread
CXXFLAGS = $(CFLAGS)

CLIENT_SOURCES := client.cpp $(wildcard network/*.cpp) $(wildcard util/*.cpp) $(wildcard request/*.cpp) $(wildcard event/*.cpp) $(wildcard command/*.cpp) $(wildcard ../lib/*.cpp)
CLIENT_OBJS := $(CLIENT_SOURCES:.cpp=.o)

SERVER_SOURCES := server.cpp $(wildcard network/*.cpp) $(wildcard util/*.cpp) $(wildcard request/*.cpp) $(wildcard event/*.cpp) $(wildcard command/*.cpp) $(wildcard ../lib/*.cpp)
SERVER_OBJS := $(SERVER_SOURCES:.cpp=.o)

FLAGS = -DERR_MSG -DINFO_MSG

IP = 127.0.0.1
PORT = 4533

include .dependsClient
include .dependsServer

client: $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(CFLAGS)

server: $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS) $(CFLAGS)

.dependsServer:
	$(CC) -MM $(SERVER_SOURCES) > .dependsServer

.dependsClient:
	$(CC) -MM $(CLIENT_SOURCES) > .dependsClient


# Standard server test
testserver:
	./server $(IP) $(PORT)

# Standard client test
testclient:
	./client $(IP) $(PORT)

clean:
	rm -rf *.o
	rm -rf ../lib/*.o
	rm -rf command/*.o
	rm -rf event/*.o
	rm -rf network/*.o
	rm -rf request/*.o