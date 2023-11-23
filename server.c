#include<stdio.h>
#define MAXCLIENTS 1024 
#define BUFFSIZE 1024
struct client_st{ 
	int sockfd; 
	char msg[MSGSIZE]; 
} 
struct server_st{ 
        int sockfd;/*server socket file */
	int clientnums;
	strcut client_st* clients[MAXCLIENT];
}

struct server_st *server;

void* create_client(int fd){
	struct client_st* client=(struct server_st*)malloc(sizeof(struct server_st));
        if(client==NULL){
                return NULL;
        }
	clients[++clientnums]=client;
	client->fd=fd;
	return client;
}
void create_tcp_server(){
	int sockfd;
	struct sockaddr_in server_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(1989);
        inet_pton(AF_INET,"127.0.0.1",server_addr.sin_addr);
	sockfd=socket(AF_INET,&server_addr,sizeof(server_addr));
	if(socketfd==-1){
                perror("socket");
                exit(1);
	}
        socklen_t len=sizeof(server_addr);
	bind(socketfd,server_addr.sin_addr,len);
        server->sockfd=sockfd;
        return sockfd;
}
void server_init(){
	
	server->sockfd=create_tcp_server();	
        memset(server,0,sizeof(*server));
	server->clientnum=0;	
        
}
void read_msg(int fd){
        char buff[BUFFSIZE];

        read(fd,buff,BUFFSIZE);
        puts(buff);
}
void send_msg(const void *msg,int fd){

}
void accept_client(int fd){
        struct sockaddr_in client_addr;
        accept(fd,&client_addr,sizeof(client_addr));
        sprintf(stdout,"IP为%s的用户已加入聊天");

}
void send_msg_to_all(){

	for(int i=0;i<server->clientnums;i++){
		if(server->clients[i]){
			struct client_st *client=server->clients[i];
			send_msg(msg,client->sockfd);	
		}
	}


}
int main(int argc,char *argv[]){
	server_init();	
        while(1){
                fd_set readset;
                FD_ZERO(&readset);
                FD_SET(server->sockfd,&readset)
                for(int i=0;i<server->clientnum;i++){
                        if(server->client[i]){
                                int fd=server->client[i]->fd; 
                                FD_SET(fd,&readset);
                        }
                }
                struct timeval tv;
                tv.tv_sec = 1; // 1 sec timeout
                tv.tv_usec = 0;
                int maxfd;
                if(server->clientnums<MAXCLIENT){
                        maxfd=clientnums+1;
                }
                int res=select(maxfd+1,readset,NULL,NULL,$tv);
                if(res<0){
                        perror("select() error");
                        exit(1);
                }
                if(FD_ISSET(server->sockfd,readset)){
                                accept_client();
                                create_client();
                }
                for(int i=0;i<server->clientnum;i++){
                        if(server->client[i]&&FD_ISSET(server->client[i]->fd,readset)){
                                msg=read_msg(server->client[i]->fd);
			       	send_msg_to_all();	
                        }
                }
        }
	

	return 0;
}
