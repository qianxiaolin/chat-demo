#include<stdio.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<time.h>
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
        if((fd=connect(sd,(void *)&addr,sizeof(addr)))<0){
                	perror("connect");
			return -1;
        }
	printf("========success connect=========");
	return fd;
}

void receive_msg(int fd){
	char buff[BUFFSIZE];
	read(fd,buff,BUFFSIZE);	
	write(stdout,buff,BUFFSIZE);
}
void send_msg(int fd){
	fprintf(fd,"input message:");
	char input[1024];
	scanf("%s",&input);
	write(fd,input,sizeof(input));
}
int main(int argc,char *argv[]){
	int sd=socket_init("localhost","1989");
	int fd=connect_server(sd);
	if(fd<0){
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
				send_msg(fd);
			}	
			if(FD_ISSET(fd,&readset)){
				receive_msg(fd);
			}
		}
	}
	return 0;
}
