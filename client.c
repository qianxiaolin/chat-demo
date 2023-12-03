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
#define BUFFSIZE 1024
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
int sock_set_nodelay(int fd){
	int flags, yes = 1;

    /* Set the socket nonblocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
	if ((flags = fcntl(fd, F_GETFL)) == -1) return -1;
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) return -1;

    /* This is best-effort. No need to check for errors. */
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
	return 0;
}
int connect_server(int sd){
	int fd;
	fprintf(stdout,"=======connect to server=========\n");
	struct sockaddr_in addr;
        addr.sin_family=AF_INET;
        addr.sin_port=htons(atoi(SERVER_PORT));
        inet_pton(AF_INET,SERVER_ADDR,&addr.sin_addr);
	int connected=0;
        if((fd=connect(sd,(void *)&addr,sizeof(addr)))==-1){
		if (errno != EINPROGRESS){
                	perror("connect");
			return -1;
		}
        }
	else{
		printf("connect fd =%d\n",fd);
	}
	printf("========success connect=========\n");
	int set=sock_set_nodelay(fd);
	if(set==-1){
		printf("set sock nodelay error\n");
	}
	return fd;
}

void receive_msg(int sockfd){
	char buff[BUFFSIZE];
	int nread=read(sockfd,buff,BUFFSIZE);	
	if(nread<0){
		perror("read remote msg error");
	}
	write(fileno(stdout),buff,nread);
}
void send_msg(int stdin_fd,int sockfd){
	printf("input message:");
	char input[1024];
	int nread;
	/*标准输入中读取数据*/
	if((nread=read(stdin_fd,input,BUFFSIZE))<0){
		perror("read");	
	}
	write(sockfd,input,sizeof(nread));
}
int main(int argc,char *argv[]){
	int sd=socket_init("localhost","1989");
	int fd=connect_server(sd);
	if(fd==-1){
		exit(1);
	}	
	int stdin_fd=fileno(stdin);
	fd_set readset;
	while(1){
		FD_ZERO(&readset);
		FD_SET(stdin_fd,&readset);
		FD_SET(fd,&readset);

		int events=select(3,&readset,NULL,NULL,NULL);
		if(events<0){
			perror("select:");
			exit(1);
		}
		else if(events){
			if(FD_ISSET(stdin_fd,&readset)){
				send_msg(stdin_fd,fd);
			}	
			if(FD_ISSET(fd,&readset)){
				printf("receive msg");
				receive_msg(fd);
			}
		}
	}
	return 0;
}
