GPP = g++ -g -pthread
FLAGS = -DERR_MSG

IP = 127.0.0.1
PORT = 4245

main: compclient compserver

# Client vanilla
compclient:
	$(GPP) client.cpp -o client

# Client debug
compclientdb:
	$(GPP) client.cpp -o client $(FLAGS)

# Server vanilla
compserver:
	$(GPP) server.cpp -o server

# Server debug
compserverdb:
	$(GPP) server.cpp -o server $(FLAGS)

# Server and Client debug
compdebug: compserverdb compclientdb

# Standard server test
testserver: main
	./server $(IP) $(PORT)

# Standard client test
testclient: main
	./client $(IP) $(PORT)

# Standard server test
testserverdb: compserverdb
	./server $(IP) $(PORT)

# Standard client test
testclientdb: compclientdb
	./client $(IP) $(PORT)