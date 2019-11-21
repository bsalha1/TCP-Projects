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
    if(argc != 4)
    {
        fprintf(stderr, "[FAIL] Must enter 2 arguments; address, port and download path\n");
        exit(EXIT_FAILURE);
    }
    char * address = argv[1];
    int port = atoi(argv[2]);
    char * filename = argv[3];
    TCPClient tcpClient = TCPClient(address, port);
    int clientSocket;
    ssize_t sizeRead = 0, totalSizeRead = 0;
    clock_t start, end;
    char buffer[BUFFER_SIZE];
    int file;



    /* Create file */
    FILE * fptr = fopen(filename, "w");
    fclose(fptr);



    /* Connect client socket to server */
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
    


    /* Open file for writing */
    file = open(filename, O_WRONLY);
    if(file == -1)
    {
        perror("File failed to open for writing");
        exit(EXIT_FAILURE);
    }



    /* Receive server packets and write to file */
    start = clock();
    do
    {
        sizeRead = recv(clientSocket, buffer, BUFFER_SIZE, 0); // Receive from server
        if(sizeRead == -1)
        {
            perror("Receive fail");
            exit(EXIT_FAILURE);
        }
        if(write(file, buffer, sizeRead) == -1) // Write to file
        {
            perror("Write fail");
            exit(EXIT_FAILURE);
        }

        totalSizeRead += sizeRead;
        fprintf(stdout, "Downloaded: %ld bytes\r", totalSizeRead);
    } while(sizeRead != 0);
    end = clock();

    close(file);
    fprintf(stdout, "\nDownload time: %lf\n", (double) (end - start) / CLOCKS_PER_SEC);

    return 0;
}
