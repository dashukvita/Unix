#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE   1
#define FALSE  0

int main(int argc , char *argv[]){
    int opt = TRUE;
    int master_socket , new_socket , client_socket[30] , max_clients = 30;
    long mesread;
    int addrlen, s, i, activity;
    struct sockaddr_in address;
    char buffer[1025];  //data buffer of 1K
    
    fd_set readfds;
    char *message = "ECHO\n";
    
    //инициализация client_socket-ов
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }
    
    //создаем master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("Socket failed.");
        exit(EXIT_FAILURE);
    }
    
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("Setsockopt's error.");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 8888 );
    
    //связываем socket с localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("Bind failed.");
        exit(EXIT_FAILURE);
    }
    
    //переводим сокет в состояние пассивного открытия
    if (listen(master_socket, 10) < 0)
    {
        perror("Listen error.");
        exit(EXIT_FAILURE);
    }
    
    //принимаем входящее соединение
    addrlen = sizeof(address);
    puts("Waiting for connections...");

    while(TRUE)
    {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);

        //добавляем клиент-сокеты
        for ( i = 0 ; i < max_clients ; i++)
        {
            s = client_socket[i];
            if(s > 0)
            {
                FD_SET( s , &readfds);
            }
        }
        //ожидаем акивности
        activity = select( max_clients + 3 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("Select error.");
        }

        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                perror("Accept error");
            }
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("Send error.");
            }

            puts("Welcome message sent successfully.");

            //добавляем новый сокет в массив сокетов
            for (i = 0; i < max_clients; i++)
            {
                s = client_socket[i];
                if (s == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                    i = max_clients;
                }
            }
        }

        for (i = 0; i < max_clients; i++)
        {
            s = client_socket[i];

            if (FD_ISSET( s , &readfds))
            {
                if ((mesread = read( s , buffer, 1024)) == 0)
                {
                    getpeername(s , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    close( s );
                    client_socket[i] = 0;
                }

                else
                {
                    buffer[mesread] = '\0';
                    for (int j = 0; j < max_clients; j++) {
                        s = client_socket[j];
                        send(s, buffer, strlen(buffer), 0);
                    }
                }
            }
        }
    }

    return 0;
}

