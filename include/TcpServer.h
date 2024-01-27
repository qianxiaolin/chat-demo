#ifndef __TCP_SERVER__
#define __TCP_SERVER__

class TcpServer
{
public:
    int create_tcp_server();
    void server_init();
    struct client_st* create_client(int fd);
    int accept_client(int fd);
private:
    int sockfd;/*server socket file */
	int clientnums;
	// struct client_st *clients[MAXCLIENTS];
};


#endif