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
#include<stdlib.h>
#include<unistd.h>
#include"socket.h"
#include<fcntl.h>


 TcpClient::TcpClient()
 {

 }
bool TcpClient::Create()
{

}
int TcpClient::ClientInit(const char *ip,const char * port)
{

}
int TcpClient::Connect(int sd){
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
	return fd;
}
int TcpClient::sock_set_nodelay(int fd){
    int flag = fcntl(sd, F_GETFL);
	if (flag == -1) {
	    perror("fcntl:");
	    exit(1);
	}

	if (fcntl(sd, F_SETFL, flag | O_NONBLOCK) == -1) {
	    perror("fcntl nonenlock");
	    exit(1);
	}
}
 TcpClient::~TcpClient()
 {

 }









