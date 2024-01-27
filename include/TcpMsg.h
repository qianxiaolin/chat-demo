#ifndef __TCPMSG__
#define __TCPMSG__


ssize_t read_nbyte(int fd,char* buff,ssize_t len);
ssize_t write_nbyte(int fd,char* buff,ssize_t buffsize);



#endif