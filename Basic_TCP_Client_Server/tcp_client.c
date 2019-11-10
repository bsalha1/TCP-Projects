#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>

#include <netinet/in.h>

int main()
{
    int network_socket;

    // arg0 = domain of socket, since its an internet socket use AF_INET
    // arg1 = type of the socket, SOCK_STREAM for TCP/Connection Socket
    // arg2 = protocol, using default protocol
    network_socket = socket(AF_INET, SOCK_STREAM, 0); // Creates a socket
    printf("Client: socket %d created.\n", network_socket);

    // Specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002); /* As a parameter, htons takes the actual port number, and it takes care of converting it to the appropriate data format
    so that the structure can understand the port number to know exactly where we are supposed to be connected to */
    server_address.sin_addr.s_addr = ((in_addr_t) 0x00000000); /* sin_addr is actually itself a structure which contains a field, so we can use another dot to get to the field
    within the structure */

    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    if(connection_status == -1)
    {
        printf("Client: socket connection failed.\n");
    }
    else
    {
        printf("Client: socket connection successful.\n");
    }

    // Receive data from server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);

    // Print data
    printf("Client: the server sent the data: %s\n", server_response);

    // Close the socket
    close(network_socket);
    
    return 0;
}