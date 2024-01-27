/*
 * 	file:server.c
 * 	descirption:server,brocast the message
 *
 *
 *
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "socket.h"
#include "rio.h"
extern struct server_st *server;
void send_msg(char *msg, size_t size, int fd)
{
    if (write(fd, msg, size) <= 0)
    {
        perror("send_msg_to_client_error");
    }
}

int TcpServer::Brocast(char *msg)
{
    for (int i = 0; i <= clientnums; i++)
    {
        if (server->clients[i])
        {
            struct client_st *send_client = server->clients[i];
            send_msg(msg, strlen(msg), send_client->sockfd);
        }
    }
}
void read_msg(int fd)
{
    char msg[1024];
    int flag = fcntl(fd, F_GETFL);

    /*set nonenlock*/
    if (flag == -1)
    {
        perror("fcntl:");
        exit(1);
    }
    if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1)
    {
        perror("fcntl nonenlock");
        exit(1);
    }

    int nread = read_nbyte(fd, msg, 1024);
    if (nread < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            ;
        else
        {
            perror("read client msg error");
            exit(1);
        }
    }
    else if (nread == 0)
    {
        printf("Client disconnected\n");
        close(fd);
        return;
    }
    else
    {
        // 读取到数据
        if (write(fileno(stdout), msg, strlen(msg)) < 0)
        {
            perror("write to std");
            exit(1);
        }
        // fflush(stdout);
        send_msg_to_all(msg);
    }
}
int TcpServer::Monitior()
{
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(server->sockfd, &readset);
    int maxfd = 0;

    /*add client fd to readset*/
    for (int i = 0; i <= server->clientnums; i++)
    {
        if (server->clients[i])
        {
            int fd = (server->clients[i])->sockfd;
            FD_SET(fd, &readset);
            if (maxfd < fd)
                maxfd = fd;
        }
    }
    if (maxfd < server->sockfd)
    {
        maxfd = server->sockfd;
    }

    struct timeval tv;
    tv.tv_sec = 1; // 1 sec timeout
    tv.tv_usec = 0;

    int res = select(maxfd + 1, &readset, NULL, NULL, NULL);
}
void TcpServer::Listen()
{
     /*listen connect*/
            if (FD_ISSET(server->sockfd, &readset))
            {
                int accept_fd = accept_client(server->sockfd);
                create_client(accept_fd);
            }

            /* listen msg*/
            for (int i = 0; i <= server->clientnums; i++)
            {
                if (server->clients[i] == NULL)
                    continue;
                int clientfd = server->clients[i]->sockfd;
                if (FD_ISSET(clientfd, &readset))
                {

                    read_msg(clientfd);
                }
            }
}

