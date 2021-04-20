/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<stdlib.h>
#include <unistd.h>
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <time.h>
#include <stdint.h>

uint64_t
os_time_get_boot_microsecond()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }

    return ((uint64_t) ts.tv_sec) * 1000 * 1000 + ((uint64_t)ts.tv_nsec) / 1000;
}

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    char *ip = "127.0.0.1";
    if (argc == 2) {
        ip = argv[1];
    }

    printf("Connect to %s\n", ip);

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( 7 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        memset(&server_reply, 0, sizeof(server_reply) );
        static int num;
        snprintf(message, sizeof(message), "Hello %d", num++);

        static uint32_t start;
        if (! start) {
            start = os_time_get_boot_microsecond() / 1000;
        }

        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        if (num % 10000 == 0) {
            printf("Server reply %ld/%d : %s\n", os_time_get_boot_microsecond()/1000 - start, num, server_reply);
        }
    }

    close(sock);
    return 0;
}
