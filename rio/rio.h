#ifndef _RIO_H_
#define _RIO_H_

typedef void ssize_t;

ssize_t read_nbyte(int fd,char* buff,ssize_t len);
ssize_t write_nbyte(int fd,char* buff,ssize_t buffsize);



#endif
