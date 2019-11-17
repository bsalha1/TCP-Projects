#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

#include <sys/socket.h>
#include <arpa/inet.h>

void process_packet(unsigned char * packet, int size);
int print_ip_header(unsigned char * packet, int size);
void print_tcp_packet(unsigned char * packet, int size);
void print_packet (unsigned char * packet, int size);

char * target_address;
int local_socket;
int num_args;
FILE * logfile;
int numPackets = 0, i, j;
struct sockaddr_in source, dest;

int main(int argc, char ** argv)
{
	int packet_size;
	struct in_addr in;
	num_args = argc;
	if(argc == 2)
	{
		target_address = argv[1];
	}
	
	unsigned char packet[65535];
	


	// Open file to log packets
	logfile = fopen("log.txt", "w");
	if(logfile == NULL) 
    {
        perror("Failed to open file for writing");
		exit(EXIT_FAILURE);
    }



	// Make sniffing socket
	local_socket = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
	if(local_socket == -1)
	{
		perror("Failed to make socket");
		exit(EXIT_FAILURE);
	}



	while(1)
	{
		// Receive a packet
		packet_size = recvfrom(local_socket, packet, 65536, 0, NULL, NULL);
		if(packet_size == -1)
		{
			perror("Failed to receive packet");
			exit(EXIT_FAILURE);
		}

		process_packet(packet, packet_size);
	}

	close(local_socket);
	printf("Finished");
	exit(EXIT_SUCCESS);
}



void process_packet(unsigned char * packet, int size)
{
	//Get the IP Header part of this packet
	struct iphdr * ipheader = (struct iphdr*) packet;
	if(ipheader->protocol == IPPROTO_TCP)
	{
		print_tcp_packet(packet, size);
	}
}



int print_ip_header(unsigned char* packet, int size)
{
	unsigned short iphdrlen;
		
	struct iphdr *ipheader = (struct iphdr *)packet;
	iphdrlen =ipheader->ihl*4;
	
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = ipheader->saddr;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = ipheader->daddr;

	if(num_args >= 2)
	{
		if(strcmp(inet_ntoa(source.sin_addr), target_address) != 0)
		{
			return -1;
		}
	}
	
	numPackets++;
	printf("TCP Packets: %d\r", numPackets);
	fflush(stdout);
	//fprintf(logfile, "\n");
	//fprintf(logfile, "IP Header\n");
	//fprintf(logfile, "   |- IP Version        : %d\n", (unsigned int) ipheader->version);
	//fprintf(logfile, "   |- IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int) ipheader->ihl,((unsigned int)(ipheader->ihl))*4);
	//fprintf(logfile, "   |- Type Of Service   : %d\n", (unsigned int) ipheader->tos);
	//fprintf(logfile, "   |- IP Total Length   : %d  Bytes(size of Packet)\n", ntohs(ipheader->tot_len));
	fprintf(logfile, "   |- Identification    : %d\n", ntohs(ipheader->id));
	//fprintf(logfile, "   |- TTL      : %d\n", (unsigned int) ipheader->ttl);
	//fprintf(logfile, "   |- Protocol : %d\n", (unsigned int) ipheader->protocol);
	//fprintf(logfile, "   |- Checksum : %d\n", ntohs(ipheader->check));
	//fprintf(logfile, "   |- Source IP        : %s\n", inet_ntoa(source.sin_addr));
	//fprintf(logfile, "   |- Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
	return 0;
}

void print_tcp_packet(unsigned char* packet, int size)
{
	unsigned short iphdrlen;
	
	struct iphdr *ipheader = (struct iphdr *)packet;
	iphdrlen = ipheader->ihl*4;
	
	struct tcphdr * tcpheader = (struct tcphdr*) (packet + iphdrlen);
			

	if(print_ip_header(packet, size) == -1)
	{
		return;
	}

	fprintf(logfile,"\n\n***********************TCP Packet*************************\n");	
		
		
	fprintf(logfile,"\n");
	fprintf(logfile,"TCP Header\n");
	//fprintf(logfile,"   |-Source Port      : %u\n", ntohs(tcpheader->source));
	//fprintf(logfile,"   |-Destination Port : %u\n", ntohs(tcpheader->dest));
	fprintf(logfile,"   |- Sequence Number    : %u\n", ntohl(tcpheader->seq));
	fprintf(logfile,"   |- Acknowledge Number : %u\n", ntohl(tcpheader->ack_seq));
	fprintf(logfile,"   |- Header Length      : %d DWORDS or %d BYTES\n", (unsigned int) tcpheader->doff, (unsigned int) tcpheader->doff*4);
	fprintf(logfile," \n|- FLAGS\n");
	fprintf(logfile,"   |- Urgent Flag          : %d\n", (unsigned int) tcpheader->urg);
	fprintf(logfile,"   |- Acknowledgement Flag : %d\n", (unsigned int) tcpheader->ack);
	fprintf(logfile,"   |- Push Flag            : %d\n", (unsigned int) tcpheader->psh);
	fprintf(logfile,"   |- Reset Flag           : %d\n", (unsigned int) tcpheader->rst);
	fprintf(logfile,"   |- Synchronise Flag     : %d\n", (unsigned int) tcpheader->syn);
	fprintf(logfile,"   |- Finish Flag          : %d\n", (unsigned int) tcpheader->fin);
	// fprintf(logfile,"   |-Window         : %d\n", ntohs(tcpheader->window));
	fprintf(logfile,"   |- Checksum       : %d\n", ntohs(tcpheader->check));
	// fprintf(logfile,"   |-Urgent Pointer : %d\n", tcpheader->urg_ptr);
	// fprintf(logfile,"\n");
	// fprintf(logfile,"                        packet Dump                         ");
	// fprintf(logfile,"\n");
		
	fprintf(logfile,"IP Header\n");
	print_packet(packet,iphdrlen);
		
	fprintf(logfile,"TCP Header\n");
	print_packet(packet+iphdrlen,tcpheader->doff * 4);
		
	fprintf(logfile,"packet Payload\n");	
	print_packet(packet + iphdrlen + tcpheader->doff*4 , (size - tcpheader->doff*4-ipheader->ihl*4) );
						
	fprintf(logfile,"\n###########################################################");
}

void print_packet(unsigned char * packet , int size)
{
	for(i = 0; i < size; i++)
	{
		if(i != 0 && i % 16 == 0)
		{
			fprintf(logfile, "         ");
			for(j = i - 16; j < i; j++)
			{
				if(packet[j] >= 32 && packet[j] <= 128) // If character has a format...
                {
					fprintf(logfile, "%c", (unsigned char) packet[j]);
                }
				else
                {
                    fprintf(logfile, ".");
                }
			}
			fprintf(logfile, "\n");
		} 
		
		if(i % 16 == 0)
		{
			fprintf(logfile, "   ");
		}
		fprintf(logfile, " %02X", (unsigned int) packet[i]);
				
		if(i == size - 1)
		{
			for(j = 0; j < 15 - i % 16; j++) 
			{
				fprintf(logfile, "   ");
			}

			fprintf(logfile, "         ");
			
			for(j = i - i % 16; j <= i; j++)
			{
				if(packet[j] >= 32 && packet[j] <= 128) // If character has a format...
				{
					fprintf(logfile, "%c", (unsigned char) packet[j]);
				}
				else 
				{
					fprintf(logfile, ".");
				}
			}
			fprintf(logfile,"\n");
		}
	}
}
