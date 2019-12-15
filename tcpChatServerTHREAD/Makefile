GPP = g++ -g -pthread
FLAGS = -DERR_MSG -DINFO_MSG

IP = 127.0.0.1
PORT = 4533

# Standard server test
testserver:
	$(GPP) server.cpp -o server
	./server $(IP) $(PORT)

# Standard client test
testclient:
	$(GPP) client.cpp -o client
	./client $(IP) $(PORT)

# Standard server test
testserverdb:
	$(GPP) server.cpp -o server $(FLAGS)
	./server $(IP) $(PORT)

# Standard client test
testclientdb:
	$(GPP) client.cpp -o client $(FLAGS)
	./client $(IP) $(PORT)