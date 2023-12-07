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
	return fd;
}

void receive_msg(int sockfd){
	char buff[BUFFSIZE];
	int nread=read(sockfd,buff,BUFFSIZE);	
	if(nread<0){
		perror("read remote msg error");
	}
	int nwrite=0;
	nwrite=write(fileno(stdout),buff,nread);
	if(nwrite<0){
		perror("write to stdout error");
	}
}
void send_msg(int stdin_fd,int sockfd){
	char input[1024];
	int nread;
	/*标准输入中读取数据*/
	if((nread=read(stdin_fd,input,BUFFSIZE))<0){
		perror("read");	
	}
	else if(nread>0){
		fprintf(stdout,"input msg:%s",input);
		int nwrite=0;
		nwrite=write(sockfd,input,nread);
		if(nwrite<0){
			perror("write to sock error");
		}
	}
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
		int maxfd=stdin_fd>fd?stdin_fd:fd;
		int events=select(maxfd+1,&readset,NULL,NULL,NULL);
		if(events<0){
			perror("select:");
			exit(1);
		}
		else if(events){
			printf("have select event activate");
			if(FD_ISSET(stdin_fd,&readset)){
			//	send_msg(stdin_fd,fd);
				char input[1024];
				int nread;
				/*标准输入中读取数据*/
				while(1){
					if((nread=read(stdin_fd,input,1))<0){
						perror("read");	
						exit(1);
						break;
					}
				}
				fprintf(stdout,"input msg:%s",input);

			}	
			if(FD_ISSET(fd,&readset)){
				printf("receive msg:");
				receive_msg(fd);
			}
		}
	}
	return 0;
}
