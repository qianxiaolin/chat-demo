#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/types.h>
ssize_t read_nbyte(int fd,char *buff,ssize_t len){
	int nread=0;
	int target=len;
	int index=0;
	while(target>0){
		if((nread=read(fd,buff+index,target))<0){
			if(errno==EINTR)
				nread=0;
			//EAGAIN:表示非阻塞下没有数据可读
			else if(errno==EAGAIN||errno==EWOULDBLOCK)
				break;
			else{
				perror("read");
				return -1;
			}
		}
		target-=nread;
		index+=nread;

	}
	return (len-target);
}
ssize_t write_nbyte(int fd,char *buff,ssize_t buffsize){
	int target=buffsize;
	int nwrite=0;
	int index=0;
	while(target){
		if(write(fd,buff+index,target)<=0){
			if(errno==EINTR)
				nwrite=0;
			else{
				perror("write");
				exit(1);
			}
		}
		target-=nwrite;
		index+=nwrite;

	}
	return buffsize-target;
}
