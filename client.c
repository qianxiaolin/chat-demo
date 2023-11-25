#include<stdio.h>
#include<sys/socket.h>
#include"proto.h" 
/*socket*/
void socket_init(const char *ip,const char * port){
	int fd;
	struct sockaddr_in client_addr;
	client_addr.sin_family=AF_INET;
	client_addr.sin_port=htons(atoi(CLIENT_ADDR));
	inet_pton(AF_INET,CLIENT_ADDR,client_addr.sin_ip);
	fd=socket(AF_INET,&client_addr,sizeof(client_addr));
	if(fd<0){
		perror("socket");
		exit(1);
	}

}
void connect_server(){
	int fd;
	sprintf(stdout,"=======connect to server=========");
	struct sockaddr_in addr;
        addr.sin_family=AF_INET;
        addr.sin_port=htons(atoi(SERVER_ADDR));
        inet_pton(AF_INET,SERVER_ADDR,addr.sin_addr);
        if((fd=conenct(sd,&addr,sizeof(addr)))<0){
                peror("connect");
                exit(1);
        }
	return fd;
}

void receive_msg(int fd){
	char buff[BUFFSIZE];
	while(1){
		int size=read(fd);
		if(size==0){
			break;	
		}
	}
	write(stdout,buff,BUFFSIZE);	
}
void send_msg(int fd){
	sprintf(stdout,"input message:");
	char input[1024];
	input=gets();
	write(fd,input,sizeof(input));
	close(fd);
}
int main(int argc,char *argv[]){
	if(argc<3){
		sprintf(stdout,"请使用:<ip> <port>格式");
		exit(1);
	}
	socket_init(argv[1],argv[2]);
	int fd=connect_server();
	while(1){
		send_msg(fd);
	}

	return 0;
}
