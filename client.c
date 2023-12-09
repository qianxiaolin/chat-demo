/*
 * 	file:client.c
 * 	description:server programe ,receive message and send message
 * 	date:2023/12/8
 * 	author:qianxiao
 *
 *
 *
 */
#include<stdio.h>
#include<errno.h>
#include <netinet/tcp.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<time.h>
#include<fcntl.h>
#include"proto.h" 
#include<string.h>
#define BUFFSIZE 1024



struct msg{
	char buff[BUFFSIZE];
	ssize_t len;
};
/*socket*/
int socket_init(const char *ip,const char * port){
	int fd;
	struct sockaddr_in client_addr;
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd<0){
		perror("socket");
		exit(1);
	}
	return fd;
}
int connect_server(int sd){
	int fd;
	fprintf(stdout,"=======connect to server=========\n");
	struct sockaddr_in addr;
        addr.sin_family=AF_INET;
        addr.sin_port=htons(atoi(SERVER_PORT));
        inet_pton(AF_INET,SERVER_ADDR,&addr.sin_addr);
	int connected=0;
        if((fd=connect(sd,(void *)&addr,sizeof(addr)))==0){
		printf("connect fd =%d\n",fd);
		        }
	else{
		if (errno != EINPROGRESS){
                	perror("connect");
			exit(1);
		}

	}
	printf("========success connect=========\n");
	return fd;
}

void receive_msg(int sockfd){
	char buff[BUFFSIZE];
	ssize_t nread=read(sockfd,buff,BUFFSIZE);	
	if(nread<=0){
		perror("read remote msg error");
		exit(1);
	}
	/*清除当前行*/
	/*光标设置在开头*/
	if(write(fileno(stdout),buff,nread)<0){
		perror("error to write to stdout");
		exit(1);
	}
}
void send_msg(int sockfd,char *buff){
	int nwrite=0;
}
int main(int argc,char *argv[]){
	int sd=socket_init("localhost","1989");
	connect_server(sd);
	int stdin_fd=fileno(stdin);
	fd_set readset;
	while(1){
		FD_ZERO(&readset);
		FD_SET(stdin_fd,&readset);
		FD_SET(sd,&readset);
		int maxfd=stdin_fd>sd?stdin_fd:sd;
		int events=select(maxfd+1,&readset,NULL,NULL,NULL);
		if(events<0){
			perror("select:");
			exit(1);
		}
		else if(events){
			if(FD_ISSET(stdin_fd,&readset)){
				/*int flag=fcntl(stdin_fd,F_GETFL);
				if(flag==-1){
					perror("fcntl:");
					exit(1);
				}

				if(fcntl(stdin_fd,F_SETFL,flag|O_NONBLOCK)==-1){
					perror("fcntl nonenlock");
					exit(1);
				}
				*/
				fflush(fileno(stdin));
				char input[1024];
				ssize_t nread=read(fileno(stdin),input,1024);
				if(nread<0){
					perror("read");
					exit(1);
				}
				if(write(sd,input,nread)<=0){
					perror("write to sock error");
				}

			}	
			if(FD_ISSET(sd,&readset)){
				receive_msg(sd);
			}
		}
	}
	return 0;
}
