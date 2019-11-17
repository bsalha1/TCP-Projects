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

int main(int argc, char ** argv)
{
    if(argc != 5)
    {
        fprintf(stderr, "Not enough arguments; ip address, port, size\n");
        exit(EXIT_FAILURE);
    }
    char buffer[1024];
    char packet[1024] = 
"0024c4dced80f6a28e31c88408004500\
003c2f7b400040061fdf80d3d42c3a60\
5c029dfc63ddf5fef9f400000000a002\
faf02bf00000020405b40402080acb42\
78ac0000000001030307";

    int local_socket;
    char * local_ip_address = "127.0.0.1";
    int local_port = atoi(argv[2]);
    struct sockaddr_in local_address;

    int server_socket;
    char * remote_ip_address = argv[3];
    int remote_port = atoi(argv[4]);
    struct sockaddr_in remote_address;

    // Address to connect from
    memset(&local_address, 0, sizeof(struct sockaddr_in));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(local_port);
    inet_pton(AF_INET, local_ip_address, &local_address.sin_addr.s_addr);


    // Address to connect to 
    memset(&remote_address, 0, sizeof(struct sockaddr_in));
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(remote_port);
    inet_pton(AF_INET, remote_ip_address, &remote_address.sin_addr.s_addr);

    fprintf(stdout, "Making local socket...\n");
    local_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(local_socket == -1)
    {
        perror("Socket create fail");
        exit(EXIT_FAILURE);
    }
    
    // if(bind(local_socket, (struct sockaddr *) &local_address, sizeof(local_address)) == -1)
    // {
    //     perror("Bind fail");
    //     exit(EXIT_FAILURE);
    // }

    // if(listen(local_socket, 64))
    // {
    //     perror("Listen fail");
    //     exit(EXIT_FAILURE);
    // }
    // fprintf(stderr, "Local socket made, bound to %s:%d and is listening\n", local_ip_address, local_port);
    
    fprintf(stderr, "Connecting to %s:%d\n", remote_ip_address, remote_port);
    if(connect(local_socket, (struct sockaddr *) &remote_address, sizeof(remote_address)) == -1)
    {
        perror("Connect fail");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "Connected to %s:%d\n", remote_ip_address, remote_port);

    write(local_socket, packet, sizeof(packet));
    fprintf(stderr, "Packet sent: %s\n", packet);

    read(local_socket, buffer, sizeof(buffer));
    fprintf(stderr, "Buffer: %s\n", buffer);


}