#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUMBER 50003
#define BUFFER_SIZE 1024
#define IP_ADDRES "255.255.255.255"
#define ERROR_MSG "Usage: mj1.exe HELLO(or BYE) YOURNAME"

int main(int argc, char *argv[])
{
	int s, optval, flag;
	socklen_t addr_len;
	struct sockaddr_in server_addr;
	char send_buf[BUFFER_SIZE];

	if(argc<2){
		printf(ERROR_MSG);
		exit(1);
	}
	
	flag=0;
	if(strcmp(argv[1], "HELLO")==0)
		++flag;
	else if(strcmp(argv[1], "BYE")==0)
		++flag;
		
	if(flag == 0){
		printf(ERROR_MSG);
		exit(1);
	}

	if((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket");
		exit(1);
	}
	optval = 1;
	if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) != 0){
		perror("sockopt");
		exit(1);
	}
//	printf("SOCKET\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUMBER);
	server_addr.sin_addr.s_addr = inet_addr(IP_ADDRES);
	addr_len = sizeof(server_addr);

	sprintf(send_buf, "%s %s", argv[1], argv[2]);
	if(sendto(s, send_buf,  strlen(send_buf), 0, (struct sockaddr *)&server_addr, addr_len)<0){
		perror("sendto");
		exit(1);
	}
//	printf("SEND[%s]\n", send_buf);

	if(close(s)<0){
		perror("close");
		exit(1);
	}

	return 0;
}

