#include<stdio.h>
#define MAXCLIENTS 1024 
struct client_st{ 
	int fd; 
	char msg[MSGSIZE]; 
} 
struct server_st{ 
        int sockfd;/*server socket file */
	int clientnums;
	strcut client_st* clients[MAXCLIENT];
}

struct server_st *server;

void create_client(fd){
	struct client_st* client=malloc();
	clients[++clientnums]=client;
	client->fd=fd;
	return 1;

}
void create_tcp_server(){
	struct sockfd;
	struct sockaddr_in server_addr;
	server_addr.sin_family;
	server_addr.sin_port=;
	sockfd=socket();
	if(socketfd==-1){
	}
	bind();
        server->sockfd=sockfd;
}
void server_init(){
	memset(server,0,sizeof(*server));
	create_tcp_server();	
	server->clientnum=0;	
}
int main(){
	server_init();	
	int readset;
	FD_SERO();
        FD_();
        for(int i=0;i<server->clientnum;i++){
                if(server->client[i]){
                        int fd=server->client[i]->fd; 
                        FD_();
                }
        }
	int res=select();
        if(res<0){
        }
        else{
                if(FD_(server->sockfd,readset)){
                        accept_client();
                        create_client();

                }
                for(int i=0;i<server->clientnum;i++){
                        if(server->client[i]){
                                if(FD_SET(server->client[i]->fd,readset)){
                                        read_msg(); 

                                }
                                
                        }
                }
        }

	return 0;
}
