#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *thread_func(void *vargp)
{
    int *connSock = vargp;
    
    puts("Connection established");
    
    int k = 0;
    char buf[1025];
    while ((k = read(*connSock, &buf, 1024)) != 0) {
        buf[k] = '\0';
        printf("%s", buf);
    }
    
    puts("Shutting down");
    
    shutdown(*connSock, SHUT_RDWR);
    close(*connSock);
    
    free(connSock);
    
    return NULL;
}

int main(int argc, char ** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s port", argv[0]);
        return 0;
    }
    
    int port = atoi(argv[1]);
    if (port <= 0) {
        fprintf(stderr, "Bad port: %s", argv[1]);
        return 0;
    }
    
    int listenSock = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
    struct sockaddr_in servaddr;
    bzero( (void *)&servaddr, sizeof(servaddr) );
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
    servaddr.sin_port = htons(port);
    bind( listenSock, (struct sockaddr *)&servaddr, sizeof(servaddr) );
    listen( listenSock, 5 );
    
    while(1)
    {
        int *connSock = malloc(sizeof(int));
        if (connSock == NULL)
        {
            perror("malloc error");
            break;
        }
        *connSock = accept( listenSock, (struct sockaddr *)NULL, (socklen_t *)NULL );
        pthread_t *thread = malloc(sizeof(pthread_t));
        if (thread == NULL)
        {
            perror("malloc error");
            break;
        }
        if (pthread_create(thread, NULL, thread_func, connSock))
        {
            perror("pthread_create error");
            break;
        }
    }
    
    
    puts("Exit");
    
    return 0;
}
