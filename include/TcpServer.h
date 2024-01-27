#ifndef __TCP_SERVER__
#define __TCP_SERVER__
#include"TcpServer.h"
#include"TcpClient.h"
#include"TcpMsg.h"
class TcpServer
{
public:
    int CreateServer();
    void ServerInit();
    struct client_st* CreateClient(int fd);
    int Accept(int fd);
    int Monitior();
    int Listen();
    int Brocast();
private:
    int sockfd;
	int clientnums;
    vector<TcpClient> clients;
	int maxclients;
};


#endif