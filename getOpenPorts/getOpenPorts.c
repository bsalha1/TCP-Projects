#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include "getOpenPorts.h"

#define TIMEOUT 10
#define MAX_PORT 65535

bool isDone = false;
pthread_t timeoutThread;
pthread_t searchThread;
pthread_mutex_t mutex;
struct sockaddr_in server_address;
int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "[FAIL] Need 1 argument; address\n");
        exit(EXIT_FAILURE);
    }

    void * numPorts;
    void * isTimedout;
    int open_ports[MAX_PORT];
    char * address = argv[1];
    pthread_mutex_init(&mutex, NULL);

    // Specify an address for the socket
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_aton(address, (struct in_addr *) &server_address.sin_addr.s_addr);

    pthread_create(&timeoutThread, NULL, timeoutFunc, NULL);
    pthread_create(&searchThread, NULL, search, open_ports);

    pthread_join(timeoutThread, &isTimedout);
    if(isTimedout)
    {
        pthread_cancel(searchThread);
        exit(EXIT_FAILURE);
    }
    pthread_join(searchThread, &numPorts);

    fprintf(stdout, "Number of open ports = %d\n", (long) numPorts);
    fprintf(stdout, "Opened ports: ");
    printIntArray(open_ports, (long) numPorts);

    exit(EXIT_SUCCESS);
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



// Returns number of open ports, puts open ports in open_ports array, must be of size 65535
void * search(void * args)
{
    int * open_ports = (int *) args;

    long num = 0;
    int port;
    int local_socket;

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
            exit(EXIT_FAILURE);
        }
    }
    fprintf(stderr, "[OK] All possible ports tested\n");
    isDone = true;
    return (void *) num;
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



void * timeoutFunc(void * args)
{
    bool isTimedout = false;
    clock_t start = clock();
    int time = 0;
    do
    {
        time = (clock() - start) / CLOCKS_PER_SEC;
        pthread_mutex_lock(&mutex);
        if(isDone)
        {
            pthread_mutex_unlock(&mutex);
            return (void *) isTimedout;
        }
        pthread_mutex_unlock(&mutex);
    } while(time < TIMEOUT);
    isTimedout = true;
    fprintf(stdout, "Timeout\n");
    return (void *) isTimedout;
}