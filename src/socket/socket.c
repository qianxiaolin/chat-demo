/*
 *
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
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<time.h>
#include<fcntl.h>
#include"socket.h" 
#include<string.h>








/*
 * 	function: 	socket_init
 * 	description: 	初始化socket,绑定端口
 *
 *
 *
 *
 *
 *
 */
int client_init(const char *ip,const char * port){
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
	return fd;
}
int sock_set_nodelay(int fd){
	int flags, yes = 1;

	if ((flags = fcntl(fd, F_GETFL)) == -1) return -1;
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) return -1;

	return 0;
}

int create_tcp_server(){
	int sockfd;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
                perror("socket");
                exit(1);
	}
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(1);
	}
	#ifdef SO_REUSEPORT
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
		perror("setsockopt SO_REUSEPORT");
		exit(1);
	}
	#endif
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(SERVER_PORT));
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        socklen_t len=sizeof(server_addr);
	if(bind(sockfd,(void *)&server_addr,len)<0){
		perror("bind");
		exit(1);
	}
	printf("-------successs create tcp server----------\n");
	if(listen(sockfd,511)<-1){
		perror("listen");
		exit(1);
	}
        return sockfd;
}
void server_init(){
	server=(struct server_st *)malloc(sizeof(struct server_st));	
        memset(server,0,sizeof(struct server_st));
	server->sockfd=create_tcp_server();	
	server->clientnums=-1;	
}
struct client_st* create_client(int fd){
	struct client_st* client=(struct client_st*)malloc(sizeof(struct client_st));
        if(client==NULL){
		fprintf(stdout,"malloc error\n");
                return NULL;
        }
	client->sockfd=fd;
	server->clientnums++;
	int clientindex=server->clientnums;
	server->clients[clientindex]=client;

	char *msg="weleco to chat!\n";
	if(write(fd,msg,sizeof(msg))<=0){
		perror("write error");
		exit(1);
	}
	return client;
}

int accept_client(int fd){
	struct sockaddr_in client_addr;
	socklen_t len=sizeof(client_addr);
	int new_fd;
	while(1){
		new_fd=accept(fd,(void *)&client_addr,&len);
		if(new_fd<=0){
			 if (errno == EINTR)
                		continue; /* Try again. */
			 else{
				perror("accept error");
				exit(1);
			 }
		}
		else{
			break;
		}
	}	
	char ipstr[1024];
	inet_ntop(AF_INET,&client_addr.sin_addr,ipstr,sizeof(ipstr));
	printf("======IP为%s的用户已加入聊天\n",ipstr);
	return new_fd;
}
void send_msg(const void *msg,ssize_t size,int fd){
	if(write(fd,msg,size)<0){
		perror("send_msg_to_client_error");
	}
}

