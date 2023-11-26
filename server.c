#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include"proto.h"
#define MAXCLIENTS 1024 
#define BUFFSIZE 1024
#define MSGSIZE  1024
struct client_st{ 
	int sockfd; 
	char ipaddr[1024];
	char msg[MSGSIZE]; 
}; 
struct server_st{ 
        int sockfd;/*server socket file */
	int clientnums;
	struct client_st* clients[MAXCLIENTS];
};

struct server_st *server;

int create_tcp_server(){
	int sockfd;
	sockfd=socket(AF_INET,SOCK_STREAM,NULL);
	if(sockfd==-1){
                perror("socket");
                exit(1);
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(SERVER_PORT));
        inet_pton(AF_INET,CLIENT_ADDR,&server_addr.sin_addr);

        socklen_t len=sizeof(server_addr);
	bind(sockfd,&server_addr.sin_addr,len);
        return sockfd;
}
void server_init(){
	
        memset(server,0,sizeof(*server));
	server->sockfd=create_tcp_server();	
	server->clientnums=-1;	
        
}
void* create_client(int fd){
	struct client_st* client=(struct client_st*)malloc(sizeof(struct client_st));
        if(client==NULL){
                return NULL;
        }
	server->clients[++clientnums]=client;
	server->client->sockfd=fd;
	return client;
}

int accept_client(int fd){
        struct sockaddr_in client_addr;
        int new_fd=accept(fd,&client_addr,sizeof(client_addr));
	if(new_fd<0){
		perror("accept error");
		exit(0);
	}
	char ipstr[1024];
	inet_ntop(AF_INET,&client_addr,&ipstr);
        fprintf(stdout,"======IP为%s的用户已加入聊天\n",ipstr);
	return new_fd;
}
void send_msg(const void *msg,int size,int fd){
	write(fd,msg,size);
	close(fd);
}
void send_msg_to_all(struct client_st *client){
	for(int i=0;i<server->clientnums;i++){
		if(server->clients[i]){
			struct client_st *send_client=server->clients[i];	
			send_msg(client->msg,MSGSIZE,send_client->sockfd);	
		}
	}
}
void read_msg(int i){
	struct client_st *client=server->clients[i];
	memset(client->msg,0,MSGSIZE);
        read(client->sockfd,client->msg,MSGSIZE);
        puts(client->msg);
}
int main(int argc,char *argv[]){
	server_init();	
        while(1){
                fd_set readset;
                FD_ZERO(&readset);
                FD_SET(server->sockfd,&readset);
                for(int i=0;i<server->clientnums;i++){
                        if(server->clients[i]){
                                int fd=server->clients[i]->sockfd; 
                                FD_SET(fd,&readset);
                        }
                }
                struct timeval tv;
                tv.tv_sec = 1; // 1 sec timeout
                tv.tv_usec = 0;
                int maxfd;
                if(server->clientnums<MAXCLIENT){
                        maxfd=server->clientnums+1;
                }
                int res=select(maxfd+1,&readset,NULL,NULL,&tv);
                if(res<0){
                        perror("select() error");
                        exit(1);
                }
                if(FD_ISSET(server->sockfd,readset)){
                                int accept_fd=accept_client(server->sockfd);
                                create_client(accept_fd);
                }
                for(int i=0;i<server->clientnums;i++){
			if(server->clients[i]==NULL) continue;
                        if(FD_ISSET(server->clients[i]->sockfd,readset)){
                                read_msg(i);
			       	send_msg_to_all(server->clients[i]);	
                        }
                }
        }
	

	return 0;
}
