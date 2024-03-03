#ifndef __TCP_CLIENT__
#define __TCP_CLIENT__

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
/*
 *
 * 	function: 	socket_init
 * 	description: 	初始化socket,绑定端口
 *  
 *
 *
 *
 *
 *
 */






class TcpClient
{
public:
    TcpClient();
    bool Open(const String url,int port);
    int sock_set_nodelay(int fd);
    ~TcpClient();
private:
    struct sockaddr_in addr;
    int sockfd; 
    char* ip;
    char* port;
};


#endif
