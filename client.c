#include<stdio.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/socket.h>
#include"proto.h" 
/*socket*/
int socket_init(const char *ip,const char * port){
	int fd;
	struct sockaddr_in client_addr;
	fd=socket(AF_INET,SOCK_STREAM,NULL);
	if(fd<0){
		perror("socket");
		exit(1);
	}
	return fd;
}
int connect_server(int sd){
	int fd;
	fprintf(stdout,"=======connect to server=========");
	struct sockaddr_in addr;
        addr.sin_family=AF_INET;
        addr.sin_port=htons(atoi(SERVER_PORT));
        inet_pton(AF_INET,SERVER_ADDR,&addr.sin_addr);
        if((fd=connect(sd,(void *)&addr,sizeof(addr)))<0){
                perror("connect");
                exit(1);
        }
	return fd;
}
/*
void receive_msg(int fd){
	char buff[BUFFSIZE];
	while(1){
		int size=read(fd);
		if(size==0){
			break;	
		}
	}
	read(stdout,buff,BUFFSIZE);	
}
*/
void send_msg(int fd){
	fprintf(stdout,"input message:");
	char input[1024];
	scanf("%s",input);
	write(fd,input,sizeof(input));
	close(fd);
}
int main(int argc,char *argv[]){
	/*
	if(argc<3){
		sprintf(stdout,"请使用:<ip> <port>格式");
		exit(1);
	}
	*/
	int sd=socket_init("localhost","1989");
	while(1){
		int fd=connect_server(sd);
		send_msg(fd);
	}

	return 0;
}
