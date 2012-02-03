#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define PORT_NUMBER 50004

int main(){
	int s, new_s, n, optval, pid, len;
	char str_buf[BUFFER_SIZE], *st, *st2;
	socklen_t addr_len;
	struct sockaddr_in server_addr, client_addr;
	
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(1);
	}
	optval = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){
		perror("setsocket");
		exit(1);
	}
	printf("ALIVE LIST\n");
//	printf("SOCKET\n");
	
	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT_NUMBER);
	if(bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("bind");
		exit(1);
	}
//	printf("BIND\n");
	if(listen(s, 10) < 0){
		perror("listen");
		exit(1);
	}
//	printf("LISTEN\n");
	
	while(1){
		addr_len = sizeof(client_addr);
		new_s = accept(s, (struct sockaddr *)&client_addr, &addr_len);
		if(new_s<0){
			perror("accept");
			exit(1);
		}
//		printf("ACCEPT\n");

		pid = fork();
		if(pid == 0){
			if(close(s)<0){
				perror("close");
				exit(1);
			}
			if((n= recv(new_s, str_buf, BUFFER_SIZE, 0)) <0){
				perror("recv");
				exit(1);
			}
//			printf("RECV\n");
			str_buf[n] = '\0';
			st = strstr(str_buf, " ");
			
			if(strncmp(str_buf, "ALIVE", 5) == 0){
				printf("%s:\t%s\n", inet_ntoa(client_addr.sin_addr), st);
			}else if(strncmp(str_buf, "MESSAGE", 7) == 0){
				st2 = strstr(str_buf, "\n\n");
				len = strlen(st+1)-strlen(st2);
				st[len+1] = '\0';
				printf("------------------------\nMESSAGE: %s\n%s\n------------------------\n", st+1,st2+2);
			}
			if(close(new_s)<0)
				perror("close");
				exit(1);		
		}else{
			if(close(new_s)<0){
				perror("close");
				exit(1);
			}
		}
	}
	
	return 0;
}
