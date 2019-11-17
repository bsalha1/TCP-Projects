#include <stdio.h>	//For standard things
#include <stdlib.h>	//malloc
#include <string.h>	//memset
#include <unistd.h>

#include <netinet/in.h>
#include <netinet/ip_icmp.h>	//Provides declarations for icmp header
#include <netinet/udp.h>	//Provides declarations for udp header
#include <netinet/tcp.h>	//Provides declarations for tcp header
#include <netinet/ip.h>	//Provides declarations for ip header

#include <sys/socket.h>
#include <arpa/inet.h>

void process_packet(unsigned char * packet, int);
void print_ip_header(unsigned char * packet, int);
void print_tcp_packet(unsigned char * packet, int);
void print_udp_packet(unsigned char * packet, int);
void print_icmp_packet(unsigned char * packet, int);
void print_data (unsigned char * packet, int);

int sock_raw;
FILE * logfile;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j;
struct sockaddr_in source,dest;

int main()
{
	int saddr_size , data_size;
	struct sockaddr saddr;
	struct in_addr in;
	
	unsigned char * buffer = (unsigned char *) malloc(65536); //Its Big!
	
	logfile = fopen("log.txt","w");
	if(logfile == NULL) 
    {
        printf("Unable to create file.");
    }
    printf("Starting...\n");

	//Create a raw socket that shall sniff
	sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
	if(sock_raw < 0)
	{
		printf("Socket Error\n");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		saddr_size = sizeof saddr;

		// Receive a packet
		data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , &saddr_size);
		if(data_size < 0)
		{
			printf("Recvfrom error , failed to get packets\n");
			exit(EXIT_FAILURE);
		}

		process_packet(buffer , data_size);
	}
	close(sock_raw);
	printf("Finished");
	exit(EXIT_SUCCESS);
}

void process_packet(unsigned char * buffer, int size)
{
	//Get the IP Header part of this packet
	struct iphdr * ipheader = (struct iphdr*) buffer;
	++total;
	switch (ipheader->protocol) //Check the Protocol and do accordingly...
	{
		case 1:  //ICMP Protocol
			++icmp;
			//PrintIcmpPacket(buffer,size);
			break;
		
		case 2:  //IGMP Protocol
			++igmp;
			break;
		
		case 6:  //TCP Protocol
			++tcp;
			print_tcp_packet(buffer , size);
			break;
		
		case 17: //UDP Protocol
			++udp;
			print_udp_packet(buffer , size);
			break;
		
		default: //Some Other Protocol like ARP etc.
			++others;
			break;
	}
	printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Others : %d   Total : %d\r", tcp, udp, icmp, igmp, others, total);
}

void print_ip_header(unsigned char* buffer, int size)
{
	unsigned short iphdrlen;
		
	struct iphdr *ipheader = (struct iphdr *)buffer;
	iphdrlen =ipheader->ihl*4;
	
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = ipheader->saddr;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = ipheader->daddr;
	
	fprintf(logfile,"\n");
	fprintf(logfile,"IP Header\n");
	fprintf(logfile,"   |-IP Version        : %d\n",(unsigned int)ipheader->version);
	fprintf(logfile,"   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)ipheader->ihl,((unsigned int)(ipheader->ihl))*4);
	fprintf(logfile,"   |-Type Of Service   : %d\n",(unsigned int)ipheader->tos);
	fprintf(logfile,"   |-IP Total Length   : %d  Bytes(size of Packet)\n",ntohs(ipheader->tot_len));
	fprintf(logfile,"   |-Identification    : %d\n",ntohs(ipheader->id));
	//fprintf(logfile,"   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
	//fprintf(logfile,"   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
	//fprintf(logfile,"   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
	fprintf(logfile,"   |-TTL      : %d\n",(unsigned int)ipheader->ttl);
	fprintf(logfile,"   |-Protocol : %d\n",(unsigned int)ipheader->protocol);
	fprintf(logfile,"   |-Checksum : %d\n",ntohs(ipheader->check));
	fprintf(logfile,"   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
	fprintf(logfile,"   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
}

void print_tcp_packet(unsigned char* buffer, int size)
{
	unsigned short iphdrlen;
	
	struct iphdr *ipheader = (struct iphdr *)buffer;
	iphdrlen = ipheader->ihl*4;
	
	struct tcphdr * tcpheader = (struct tcphdr*) (buffer + iphdrlen);
			
	fprintf(logfile,"\n\n***********************TCP Packet*************************\n");	
		
	print_ip_header(buffer,size);
		
	fprintf(logfile,"\n");
	fprintf(logfile,"TCP Header\n");
	fprintf(logfile,"   |-Source Port      : %u\n",ntohs(tcpheader->source));
	fprintf(logfile,"   |-Destination Port : %u\n",ntohs(tcpheader->dest));
	fprintf(logfile,"   |-Sequence Number    : %u\n",ntohl(tcpheader->seq));
	fprintf(logfile,"   |-Acknowledge Number : %u\n",ntohl(tcpheader->ack_seq));
	fprintf(logfile,"   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcpheader->doff,(unsigned int)tcpheader->doff*4);
	//fprintf(logfile,"   |-CWR Flag : %d\n",(unsigned int)tcpheader->cwr);
	//fprintf(logfile,"   |-ECN Flag : %d\n",(unsigned int)tcpheader->ece);
	fprintf(logfile,"   |-Urgent Flag          : %d\n",(unsigned int)tcpheader->urg);
	fprintf(logfile,"   |-Acknowledgement Flag : %d\n",(unsigned int)tcpheader->ack);
	fprintf(logfile,"   |-Push Flag            : %d\n",(unsigned int)tcpheader->psh);
	fprintf(logfile,"   |-Reset Flag           : %d\n",(unsigned int)tcpheader->rst);
	fprintf(logfile,"   |-Synchronise Flag     : %d\n",(unsigned int)tcpheader->syn);
	fprintf(logfile,"   |-Finish Flag          : %d\n",(unsigned int)tcpheader->fin);
	fprintf(logfile,"   |-Window         : %d\n",ntohs(tcpheader->window));
	fprintf(logfile,"   |-Checksum       : %d\n",ntohs(tcpheader->check));
	fprintf(logfile,"   |-Urgent Pointer : %d\n",tcpheader->urg_ptr);
	fprintf(logfile,"\n");
	fprintf(logfile,"                        DATA Dump                         ");
	fprintf(logfile,"\n");
		
	fprintf(logfile,"IP Header\n");
	print_data(buffer,iphdrlen);
		
	fprintf(logfile,"TCP Header\n");
	print_data(buffer+iphdrlen,tcpheader->doff*4);
		
	fprintf(logfile,"Data Payload\n");	
	print_data(buffer + iphdrlen + tcpheader->doff*4 , (size - tcpheader->doff*4-ipheader->ihl*4) );
						
	fprintf(logfile,"\n###########################################################");
}

void print_udp_packet(unsigned char *buffer , int size)
{
	
	unsigned short iphdrlen;
	
	struct iphdr *ipheader = (struct iphdr *)buffer;
	iphdrlen = ipheader->ihl*4;
	
	struct udphdr *udph = (struct udphdr*)(buffer + iphdrlen);
	
	fprintf(logfile,"\n\n***********************UDP Packet*************************\n");
	
	print_ip_header(buffer,size);			
	
	fprintf(logfile,"\nUDP Header\n");
	fprintf(logfile,"   |-Source Port      : %d\n" , ntohs(udph->source));
	fprintf(logfile,"   |-Destination Port : %d\n" , ntohs(udph->dest));
	fprintf(logfile,"   |-UDP Length       : %d\n" , ntohs(udph->len));
	fprintf(logfile,"   |-UDP Checksum     : %d\n" , ntohs(udph->check));
	
	fprintf(logfile,"\n");
	fprintf(logfile,"IP Header\n");
	print_data(buffer , iphdrlen);
		
	fprintf(logfile,"UDP Header\n");
	print_data(buffer+iphdrlen , sizeof udph);
		
	fprintf(logfile,"Data Payload\n");	
	print_data(buffer + iphdrlen + sizeof udph ,( size - sizeof udph - ipheader->ihl * 4 ));
	
	fprintf(logfile,"\n###########################################################");
}

void print_icmp_packet(unsigned char* buffer , int size)
{
	unsigned short iphdrlen;
	
	struct iphdr *ipheader = (struct iphdr *)buffer;
	iphdrlen = ipheader->ihl*4;
	
	struct icmphdr *icmph = (struct icmphdr *)(buffer + iphdrlen);
			
	fprintf(logfile,"\n\n***********************ICMP Packet*************************\n");	
	
	print_ip_header(buffer , size);
			
	fprintf(logfile,"\n");
		
	fprintf(logfile,"ICMP Header\n");
	fprintf(logfile,"   |-Type : %d",(unsigned int)(icmph->type));
			
	if((unsigned int)(icmph->type) == 11) 
		fprintf(logfile,"  (TTL Expired)\n");
	else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY) 
		fprintf(logfile,"  (ICMP Echo Reply)\n");
	fprintf(logfile,"   |-Code : %d\n",(unsigned int)(icmph->code));
	fprintf(logfile,"   |-Checksum : %d\n",ntohs(icmph->checksum));
	//fprintf(logfile,"   |-ID       : %d\n",ntohs(icmph->id));
	//fprintf(logfile,"   |-Sequence : %d\n",ntohs(icmph->sequence));
	fprintf(logfile,"\n");

	fprintf(logfile,"IP Header\n");
	print_data(buffer,iphdrlen);
		
	fprintf(logfile,"UDP Header\n");
	print_data(buffer + iphdrlen , sizeof icmph);
		
	fprintf(logfile,"Data Payload\n");	
	print_data(buffer + iphdrlen + sizeof icmph , (size - sizeof icmph - ipheader->ihl * 4));
	
	fprintf(logfile,"\n###########################################################");
}

void print_data (unsigned char* data , int size)
{
	
	for(i=0; i < size ; i++)
	{
		if(i != 0 && i % 16 == 0)   //if one line of hex printing is complete...
		{
			fprintf(logfile,"         ");
			for(j = i - 16; j < i; j++)
			{
				if(data[j] >= 32 && data[j] <= 128)
                {
					fprintf(logfile, "%c", (unsigned char) data[j]); //if its a number or alphabet
                }
				else
                {
                    fprintf(logfile, "."); //otherwise print a dot
                }
			}
			fprintf(logfile,"\n");
		} 
		
		if(i%16==0) fprintf(logfile,"   ");
			fprintf(logfile," %02X",(unsigned int)data[i]);
				
		if(i == size - 1)  //print the last spaces
		{
			for(j = 0; j < 15 - i % 16; j++) fprintf(logfile,"   "); //extra spaces
			
			fprintf(logfile,"         ");
			
			for(j=i-i%16 ; j<=i ; j++)
			{
				if(data[j]>=32 && data[j]<=128) fprintf(logfile,"%c",(unsigned char)data[j]);
				else fprintf(logfile,".");
			}
			fprintf(logfile,"\n");
		}
	}
}