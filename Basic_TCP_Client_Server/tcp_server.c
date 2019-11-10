#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <fcntl.h>

#include <unistd.h>

#include <netinet/in.h>

#define FILE_TO_SEND "server_data.txt"

int main()
{
    int fd;
    int len;
    int port = 9002;
    int remain_data;
    int sent_bytes = 0;
    int client_socket;
    long offset;
    struct stat file_stat;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    socklen_t sock_len;
    char file_size[256];

    // Create socket ... //
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("Server: socket failed to be created.\n");
        return EXIT_FAILURE;
    }
    printf("Server: socket %d created.\n", server_socket);



    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = ((in_addr_t) 0x00000000);



    // Bind socket to IP and port
    bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    printf("Server: socket %d has been bound to address: 0.0.0.0:%d\n", server_socket, port);

    
    
    while(1)
    {   
        // Listen... //
        if (listen(server_socket, 5) == -1)
        {
            printf("Server: server socket %d failed to initiate listening.\n", server_socket);
            return EXIT_FAILURE;
        }
        printf("Server: server socket %d is listening...\n", server_socket);



        // Open file... //
        fd = open(FILE_TO_SEND, O_RDONLY);
        if (fd == -1)
        {
            printf("Server: error opening file.\n");
            return EXIT_FAILURE;
        }



        // Get file statistics... //
        if (fstat(fd, &file_stat) == -1)
        {
            printf("Error getting fstat.\n");
            return EXIT_FAILURE;
        }
        printf("Server: file size = %ld bytes\n", file_stat.st_size);
        
        

        sock_len = sizeof(struct sockaddr_in);
        // Accept client socket... //
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &sock_len);
        if (client_socket == -1)
        {
            printf("Server: client socket %d not accepted.\n", client_socket);
            return EXIT_FAILURE;
        }
        printf("Server: client socket %d accepted.\n", client_socket);



        sprintf(file_size, "%ld", file_stat.st_size);



        // Send server data to client... //
        len = send(client_socket, file_size, sizeof(file_size), 0);
        if (len == -1)
        {
            printf("Server: server data failed to be sent to client socket %d.\n", client_socket);
            return EXIT_FAILURE;
        }
        printf("Server: server data sent %d bytes for the size.\n", len);

        offset = 0;
        remain_data = file_stat.st_size;



        // Send the data... //
        while (((sent_bytes = sendfile(client_socket, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
        {
                if(remain_data == 0)
                {
                    printf("Server: file has been completely sent.\n");
                }
                else
                {
                    printf("Server: server sent %d/%d bytes\n", remain_data, sent_bytes);
                    remain_data -= sent_bytes;
                }
                
        }
    }

    close(server_socket);
    close(client_socket);

    return EXIT_SUCCESS;
}