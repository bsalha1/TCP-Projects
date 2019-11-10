#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <signal.h>

#include "enums.h"

#include "../tcpLib/tcpServer.h"

void exitHandler(int signal);

TCPServer * tcpServer;
int file;

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "[FAIL] Must enter two arguments; server address and port\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, exitHandler);
    tcpServer = new TCPServer(argv[1], atoi(argv[2]));
    char * filename = argv[3];
    int serverSocket, clientSocket;
    socklen_t socketLength = sizeof(struct sockaddr_in);
    time_t start, end;
    sockaddr_in clientAddr;
    ssize_t len, sentBytes = 0, totalSentBytes = 0;
    off_t offset = 0;

    /* Create server socket, bind, listen and accept client */
    try
    {
        serverSocket = tcpServer->makeSocket();
        tcpServer->bindSocket(serverSocket);
        tcpServer->listenOnSocket(serverSocket, 10);
        clientSocket = tcpServer->acceptClient(serverSocket, (struct sockaddr *) &clientAddr, &socketLength);
    }
    catch(exception& e)
    {
        printf("exception\n");
        fprintf(stdout, "%s", e.what());
        exit(EXIT_FAILURE);
    }

    /* Open file */
    file = open(filename, O_RDONLY);
    if (file == -1)
    {
        fprintf(stderr, "[FAIL] Error opening file %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Sending file data */
    time(&start);
    do
    {
        sentBytes = sendfile(clientSocket, file, &offset, BUFFER_SIZE);
        if(sentBytes == -1)
        {
            fprintf(stdout, "[FAIL] Failed to send data to client: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        totalSentBytes += sentBytes;
        fprintf(stdout, "Sent: %ld bytes", totalSentBytes);
        fprintf(stdout, "\r");
    } while(sentBytes != 0);
    time(&end);
    
    fprintf(stdout, "\nDownload time: %.2lf seconds\n", (double) (end - start));
    
    exitHandler(0);

    return 0;
}



/*
    Handles interrupt signal
*/
void exitHandler(int signal)
{
    close(file);
    delete tcpServer;
}
