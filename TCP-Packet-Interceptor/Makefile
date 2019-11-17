GCC = gcc interceptor.c -g -o interceptor
IP = 58.96.92.2

test1: 
	$(GCC)
	gnome-terminal -x bash -c "tail -f log.txt; exec bash"
	./interceptor $(IP)

