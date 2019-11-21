#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <netinet/in.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>


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
    socklen_t socketLength;
    clock_t start, end;
    sockaddr_in clientAddr;
    ssize_t len, sentBytes = 0, totalSentBytes = 0;
    off_t offset = 0;



    /* Open file */
    file = open(filename, O_RDONLY);
    if (file == -1)
    {
        perror("File failed to open for writing");
        exit(EXIT_FAILURE);
    }



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
        fprintf(stdout, "%s", e.what());
        exit(EXIT_FAILURE);
    }



    /* Send file data via packets to client */
    start = clock();
    do
    {
        sentBytes = sendfile(clientSocket, file, &offset, BUFFER_SIZE);
        if(sentBytes == -1)
        {
            perror("Send fail");
            exit(EXIT_FAILURE);
        }
        totalSentBytes += sentBytes;
        fprintf(stdout, "Sent: %ld bytes\r", totalSentBytes);
    } while(sentBytes != 0);
    end = clock();

    close(file);
    fprintf(stdout, "\nDownload time: %lf seconds\n", (double) (end - start) / CLOCKS_PER_SEC);
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
