GPP1 = gcc getOpenPorts.c -g -o getOpenPorts -pthread
IP = 128.211.212.44

main:
	$(GPP1)

testinet: main
	./getOpenPorts $(IP)

testlocal: main
	./getOpenPorts 127.0.0.1