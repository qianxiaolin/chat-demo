#ifndef __TCPMSG__
#define __TCPMSG__
#include<unstd.h>
class TcpMsg
{
public:
    
    TcpMsg();
    int ReceiveMsg();
    int SendMsg();
    ~TcpMsg();
private:
    ssize_t read_nbyte(int fd,char* buff,ssize_t len);
    ssize_t write_nbyte(int fd,char* buff,ssize_t buffsize);
    char buff[BUFFSIZE];
	ssize_t len;
};

TcpMsg::TcpMsg(/* args */)
{
}

TcpMsg::~TcpMsg()
{
}
void receive_msg(int sockfd){
	char buff[BUFFSIZE];
	ssize_t nread=read_nbyte(sockfd,buff,BUFFSIZE);	
	if(nread<=0){
		if(nread==0){
			printf("server disconnected");
		}
		else{
			perror("read remote msg error");
		}
		exit(1);
	}
	if(write(fileno(stdout),buff,nread)<0){
		perror("error to write to stdout");
		exit(1);
	}
}




#endif