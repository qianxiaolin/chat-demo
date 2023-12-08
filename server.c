#include<string.h>
#include <netinet/tcp.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<fcntl.h>
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
	struct client_st *clients[MAXCLIENTS];
};

struct server_st *server;
int sock_set_nodelay(int fd){
	int flags, yes = 1;

    /* Set the socket nonblocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
	if ((flags = fcntl(fd, F_GETFL)) == -1) return -1;
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) return -1;

    /* This is best-effort. No need to check for errors. */
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
        memset(server,0,sizeof(*server));
	server->sockfd=create_tcp_server();	
	server->clientnums=-1;	
}
void* create_client(int fd){
	struct client_st* client=(struct client_st*)malloc(sizeof(struct client_st));
        if(client==NULL){
		fprintf(stdout,"malloc error\n");
                return NULL;
        }
	printf("success create client\n");
	client->sockfd=fd;
	server->clientnums++;
	int clientindex=server->clientnums;
	server->clients[clientindex]=client;

	char *msg="weleco to chat";
	if(write(fd,msg,sizeof(msg))<0){
		perror("write error");
		exit(1);
	}
	return client;
}

int accept_client(int fd){
	printf("=========try_to_accept_client========\n");
	fflush(stdout);
	struct sockaddr_in client_addr;
	socklen_t len=sizeof(client_addr);
	int new_fd;
	while(1){
		new_fd=accept(fd,(void *)&client_addr,&len);
		if(new_fd==-1){
			 if (errno == EINTR)
                		continue; /* Try again. */
			 else{
				perror("accept error");
			 }
		}
		else{
			break;
		}
	}	
	char ipstr[1024];
	inet_ntop(AF_INET,&client_addr.sin_addr,ipstr,sizeof(ipstr));
	printf("======IP为%s的用户已加入聊天\n",ipstr);
//	sock_set_nodelay(new_fd);	
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
void read_msg(int fd){
	char msg[1024];
	int nread;
	int flag=fcntl(fd,F_GETFL);
	if(flag==-1){
		perror("fcntl:");
		exit(1);
	}

	if(fcntl(fd,F_SETFL,flag|O_NONBLOCK)==-1){
		perror("fcntl nonenlock");
		exit(1);
	}

	if((nread=read(fd,msg,1024))<=0){
			perror("read client msg error");
			exit(1);
	}
        puts(msg);
}
int main(int argc,char *argv[]){
	server_init();	
        while(1){
        	fd_set readset;
                FD_ZERO(&readset);
                FD_SET(server->sockfd,&readset);
		int maxfd=0;
                for(int i=0;i<=server->clientnums;i++){
                        if(server->clients[i]){
                                int fd=(server->clients[i])->sockfd; 
				if(maxfd<fd)
					maxfd=fd;
                        }
                }
                struct timeval tv;
                tv.tv_sec = 1; // 1 sec timeout
                tv.tv_usec = 0;
                if(maxfd<server->sockfd){
                        maxfd=server->sockfd;
                }
                int res=select(maxfd+1,&readset,NULL,NULL,NULL);
                if(res==-1){
                        perror("select() error");
                        exit(1);
                }
		else if(res){
			if(FD_ISSET(server->sockfd,&readset)){
				int accept_fd=accept_client(server->sockfd);
				create_client(accept_fd);	
                                FD_SET(accept_fd,&readset);
				printf("client fd is %d",accept_fd);
			}
			printf("listen a file change\n");
			printf("num of client:%d\n",server->clientnums);
			for(int i=0;i<=server->clientnums;i++){
				if(server->clients[i]==NULL) continue;
				int clientfd=server->clients[i]->sockfd;
				if(FD_ISSET(clientfd,&readset)){
					printf("have file change\n");
					
					printf("client fd is %d\n",clientfd);
					read_msg(clientfd);
					send_msg_to_all(server->clients[i]);	
				}
			}

			
		}
	
	}
	return 0;
}
