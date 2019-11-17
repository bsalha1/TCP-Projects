#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../tcpLib/tcp_client.h"

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, "Must enter two arguments; address and port.\n");
        return EXIT_FAILURE;
    }
    char * address = argv[1];
    int port = atoi(argv[2]);
    int clientSocket;
    char message[2048];
    char username[32];
    char password[256];
    TCPClient tcpClient = TCPClient(address, port);

    
    clientSocket = tcpClient.makeSocket();
    tcpClient.connectToServer(clientSocket);

    // Get and send username ... //
    cout << "Username: ";
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; // Cut off \n
    tcpClient.sendData(username, sizeof(username), clientSocket);

    // Get message
    while(1)
    {
        
        cout << "Message: ";
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Cut off \n
        tcpClient.sendData(message, sizeof(message), clientSocket);
        /*cout << "Password: ";
        fgets(password, sizeof(password), stdin);
        */
        
    
        // Send message
        //tcp_client.sendData(password, sizeof(password), socket1);

        /*if(tcp_client.connectToServer(&socket2) == -1)
        {
            return -1;
        }

        // Prompt for Message ... //
        cout << "Message: ";
        fgets(message, sizeof(message), stdin);

        // Send Message to Server ... //
        tcp_client.sendData(message, sizeof(message), socket2);*/
    }
    return EXIT_SUCCESS;
}