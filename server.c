#include<stdio.h>

struct client_st{
	int fd;
	char msg[MSGSIZE];
}
struct server_st{
	int socket;/*server socket file */
	int clientnums;
	strcut client_st* clients[MAXCLIENT];
}

struct server_st server;

void create_client(fd){
	struct client_st* client=malloc();
	clients[++clientnums]=client;
	client->fd=fd;
	return 1;

}

void server_init(){
	struct sockfd;
	struct sockaddr_in server_addr;
	server_addr.sin_family;
	server_addr.sin_port=;
	sockfd=socket();
	server->fd=fd;
	bind();
	server->clientnum=0;
		
}
int main(){
	server_init();	


	while(1){
	fd=accept();
	create_client(fd);
	
	}

	return 0;
}
