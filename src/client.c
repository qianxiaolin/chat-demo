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
#include"proto.h" 




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
				}*/
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
