COMP1 = g++ server.cpp -o server -DERR_MSG -DINFO_MSG
COMP2 = g++ client.cpp -o client -DERR_MSG -DINFO_MSG

testserver:
	$(COMP1)
	./server 128.211.212.44 80 files/Loader.class

testclient:
	$(COMP2)
	./client 128.211.212.44 80 downloads/Loader.class