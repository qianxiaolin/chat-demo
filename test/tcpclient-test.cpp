int main(int argc,char *argv[]){
	int sd=client_init("localhost","1989");
	connect_server(sd);
	int stdin_fd=fileno(stdin);
	fd_set readset;
	
	


	while(1){
		fflush(stdin);
		FD_ZERO(&readset);
		FD_SET(stdin_fd,&readset);
		FD_SET(sd,&readset);
		int maxfd=stdin_fd>sd?stdin_fd:sd;
		int events=select(maxfd+1,&readset,NULL,NULL,NULL);
		if(events<0){
			perror("select:");
			exit(1);
		}
		else if(events){
			if(FD_ISSET(stdin_fd,&readset)){
				
				char input[1024];
				ssize_t nread=read_nbyte(fileno(stdin),input,1024);
				if(nread<=0){
					if(nread==0){
						printf("conenct error");
					}
					else{
						perror("read");
					}
					exit(1);
				}
				input[nread]='\0';
				if(write(sd,input,nread)<=0){
					perror("write to error");
					exit(1);
				}
				/*if(write(sd,input,nread)<=0){
					if (errno == EAGAIN || errno == EWOULDBLOCK) {
					// 缓冲区已满，稍后再尝试写入
						continue;
					} 
					else {
						perror("write to sock error");
						exit(1);
				    	}
				}*/

			}	
			if(FD_ISSET(sd,&readset)){
				receive_msg(sd);
			}
		}
	}
	return 0;
}
