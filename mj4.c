#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUMBER 50004
#define BUFFER_SIZE 8000
#define ERROR_MSG "Usage: mj4.exe IPADDRESS YOURNAME"

int main(int argc, char *argv[])
{
	int s;
	socklen_t addr_len;
	struct sockaddr_in server_addr;
	char str_buf[BUFFER_SIZE], send_buf[BUFFER_SIZE];

	if(argc<2){
		printf(ERROR_MSG);
		exit(1);
	}

	printf("Please Input Message...\n");
	scanf("%s", &str_buf);

	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(1);
	}
//	printf("SOCKET\n");

	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	server_addr.sin_port=htons(PORT_NUMBER);

	if(connect(s,(struct sockaddr *)&server_addr, sizeof(server_addr))<0){
		perror("connect");
		exit(1);
	}
//	printf("CONNECT\n");

	sprintf(send_buf, "MESSAGE %s\n\n%s", argv[2], str_buf);
	if(sendto(s, send_buf,  strlen(send_buf), 0, (struct sockaddr *)&server_addr, addr_len)<0){
		perror("sendto");
		exit(1);
	}
	printf("SEND\n----------------\n%s\n-----------------\n", send_buf);

	if(close(s)<0){
		perror("close");
		exit(1);
	}

	return 0;
}

