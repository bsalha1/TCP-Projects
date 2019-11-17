GPP1 = g++ server.cpp -g -o server
GPP2 = g++ client.cpp -g -o client

main:
	$(GPP1)
	$(GPP2)
	

testserverdb:
	$(GPP1)
	./server 127.0.0.1 80 
	#2> server_error.txt

testclientdb:
	$(GPP2)
	./client 127.0.0.1 80 
	#2> client_error.txt

testserver:
	$(GPP1)
	./server 127.0.0.1 80 2> server_error.txt

testclient:
	$(GPP2)
	./client 127.0.0.1 80 2> client_error.txt