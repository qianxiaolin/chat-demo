/*
 * 	file:client.c
 * 	description:server programe ,receive message and send message
 * 	date:2023/12/8
 * 	author:qianxiao
 *
 *
 *
 */

#ifndef _PROTO_
#define _PROTO_

#define CLIENT_ADDR 	"127.0.0.1"

#define SERVER_ADDR     "127.0.0.1"
#define SERVER_PORT 	"7711"
#define BUFFSIZE 1024

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
struct msg{
	char buff[BUFFSIZE];
	ssize_t len;
};
/*socket*/





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
int client_init(const char *ip,const char * port);
int connect_server(int sd);

void receive_msg(int sockfd);
int sock_set_nodelay(int fd);

int create_tcp_server();
void server_init();
struct client_st* create_client(int fd);
int accept_client(int fd);
void send_msg(const void *msg,ssize_t size,int fd);
void send_msg_to_all(char *msg,ssize_t nread);
void read_msg(int fd);
#endif
