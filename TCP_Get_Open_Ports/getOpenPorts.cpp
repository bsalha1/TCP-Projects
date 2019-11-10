#include <iostream>
#include <thread>

#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <netinet/in.h>

#define TIMEOUT 10
#define MAX_PORT 65535

using namespace std;

int getOpenPorts(struct sockaddr_in, int *);
int makeSocket();
void printIntArray(int * arr, int length);
int timeoutThreadFunc();

bool doneFlag = false;
int main(int argc, char ** argv)
{
    int num;
    int local_socket;
    int open_ports[MAX_PORT];
    char * address;
    struct sockaddr_in server_address;

    if(argc != 2)
    {
        fprintf(stderr, "[FAIL] Need 1 argument; address\n");
        exit(EXIT_FAILURE);
    }
    address = argv[1];

    // Create socket
    local_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(local_socket == -1)
    {
        fprintf(stderr, "[FAIL] Local socket failed to be created\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "[OK] Local socket %d created\n", local_socket);

    // Specify an address for the socket
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_aton(address, (struct in_addr *) &server_address.sin_addr.s_addr);


    num = getOpenPorts(server_address, open_ports);
    
    fprintf(stdout, "Number of open ports = %d\n", num);
    fprintf(stdout, "Opened ports: ");
    printIntArray(open_ports, num);


    // Close the socket
    if(close(local_socket) == -1)
    {
        fprintf(stderr, "[FAIL] Local socket %d failed to close: %s\n", local_socket, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}

// Returns number of open ports, puts open ports in open_ports array, must be of size 65535
int getOpenPorts(struct sockaddr_in server_address, int * open_ports)
{
    int num = 0;
    int port;
    int local_socket;
    thread timeoutThread = thread(timeoutThreadFunc);

    fprintf(stdout, "[OK] Getting open ports...\n");
    for(port = 0; port < MAX_PORT; port++)
    {
        local_socket = makeSocket();
        server_address.sin_port = htons(port);
        if(connect(local_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != -1)
        {
            open_ports[num] = port;
            num++;
        }
        if(close(local_socket) == -1)
        {
            fprintf(stderr, "[FAIL] Socket %d failed to close: %s\n", local_socket, strerror(errno));
            timeoutThread.join();
            exit(EXIT_FAILURE);
        }
    }
    fprintf(stderr, "[OK] All possible ports tested\n");
    timeoutThread.detach();
    
    return num;
}

int makeSocket()
{
    int optVal = 1;
    int local_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(local_socket == -1)
    {
        fprintf(stderr, "[FAIL] Local socket failed to be created: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    setsockopt(local_socket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(int));
    return local_socket;
}

void printIntArray(int * arr, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int timeoutThreadFunc()
{
    int i;
    for(i = 0; i < TIMEOUT; i++)
    {
        sleep(1);
    }
    fprintf(stderr, "[FAIL] Timeout\n");
    exit(EXIT_FAILURE);
}   
