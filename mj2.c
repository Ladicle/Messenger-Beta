#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUMBER 50003
#define PORT_NUMBER2 50004
#define BUFFER_SIZE 1024
#define IP_ADDRES "255.255.255.255"
#define ERROR_MSG "Usage: mj1.exe HELLO(or BYE) YOURNAME"

int main()
{
	int udp_s, tcp_s, optval, n;
	socklen_t addr_len;
	struct sockaddr_in server_addr, client_addr;
	char str_buf[BUFFER_SIZE], *st;

	if((udp_s = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket");
		exit(1);
	}
	optval = 1;
	if(setsockopt(udp_s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) != 0){
		perror("sockopt");
		exit(1);
	}
//	printf("UDP_SOCKET\n");

	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT_NUMBER);
	if(bind(udp_s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("bind");
		exit(1);
	}
//	printf("BIND\n");

	while(1)
	{
		addr_len = sizeof(client_addr);
		if((n = recvfrom(udp_s, str_buf, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len)) < 0){
			perror("recv");
		}
//		printf("RECV\n");
		str_buf[n] = '\0';
		st = strstr(str_buf, " ");
		++st;
		
		if(strncmp(str_buf, "HELLO", 5) == 0){
			printf("HELLO\t%s:\t%s\n", inet_ntoa(client_addr.sin_addr), st);
			if((tcp_s = socket(AF_INET, SOCK_STREAM, 0)) < 0){
				perror("socket");
				exit(1);
			}
			client_addr.sin_port = htons(50004);
//			printf("TCP_SOCKET\n");
			if(connect(tcp_s,(struct sockaddr *)&client_addr, sizeof(client_addr))<0){
				perror("connect");
				exit(1);
			}
			sprintf(str_buf, "ALIVE %s", st);
//			printf("CONNECT\n");
			if(send(tcp_s, str_buf,  strlen(str_buf), 0)<0){
				perror("sendto");
				exit(1);
			}
//			printf("SEND\n");
			if(close(tcp_s)<0){
				perror("close");
				exit(1);
			}
			
		}else if(strncmp(str_buf, "BYE", 3) == 0){
			printf("BYE\t%s:\t%s\n", inet_ntoa(client_addr.sin_addr), st);
		}
	}
	return 0;
}

