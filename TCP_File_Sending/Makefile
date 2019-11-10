COMP1 = g++ server.cpp -o server -DERR_MSG -DFORMATTED_ERROR
COMP2 = g++ client.cpp -o client -DERR_MSG -DFORMATTED_ERROR

main:
	$(COMP1)
	$(COMP2)

testclient: main
	./client 127.0.0.1 80

testserver: main
	./server 127.0.0.1 80 bigFile.iso
