GPP1 = g++ getOpenPorts.cpp -g -o getOpenPorts -pthread

main:
	$(GPP1)

testinet: main
	./getOpenPorts 10.192.37.247

testlocal: main
	./getOpenPorts 127.0.0.1