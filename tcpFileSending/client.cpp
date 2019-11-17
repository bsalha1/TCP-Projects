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

#include "enums.h"
#include "../tcpLib/tcpClient.h"

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        fprintf(stderr, "[FAIL] Must enter 2 arguments; address and port\n");
        exit(EXIT_FAILURE);
    }
    char * address = argv[1];
    int port = atoi(argv[2]);
    char * filename = "clientFile.iso";
    TCPClient tcpClient = TCPClient(address, port);
    int clientSocket;
    ssize_t sizeRead = 0, totalSizeRead = 0;
    time_t start, end;
    char buffer[BUFFER_SIZE];
    int file;

    /*
        Connect client socket to server
    */
    try
    {   
        clientSocket = tcpClient.makeSocket();
        tcpClient.connectToServer(clientSocket);   
    }
    catch(exception e)
    {
        fprintf(stdout, "%s", e.what());
        exit(EXIT_FAILURE);
    }
    
    /*
        Open file for writing
    */
    file = open(filename, O_WRONLY);
    if(file == -1)
    {
        perror("File failed to open for writing");
        exit(EXIT_FAILURE);
    }

    time(&start);
    do
    {
        sizeRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if(sizeRead == -1)
        {
            perror("Read fail");
            exit(EXIT_FAILURE);
        }
        if(write(file, buffer, sizeof(buffer)) == -1)
        {
            perror("Write fail");
            exit(EXIT_FAILURE);
        }

        totalSizeRead += sizeRead;
        fprintf(stdout, "Downloaded: %ld bytes", totalSizeRead);
        fprintf(stdout, "\r");
    } while(sizeRead != 0);
    time(&end);

    fprintf(stdout, "\nDownload time: %.2lf\n", (double) (end - start));
    close(file);

    return 0;
}
