#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd;
    //Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error: Could not create socket file descriptor");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    //Bind the socket to the port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("Error: Could not bind the socket to the port 8080");
        exit(EXIT_FAILURE);
    }

    //Listen for a connection
    if(listen(server_fd, 5)<0)
    {
        perror("Error: Listening for a connection");
        exit(EXIT_FAILURE);
    }

    //Accept, read, and write
    //Loop to accept connections
    int new_socket;
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("Error: Could not accept connection");
            exit(EXIT_FAILURE);
        }

        //Reading
        char buffer[30000] = {0};
        int read_bytes = read(new_socket , buffer, 30000);
        //printf("%s\n",buffer );

        //Writing
        char *hello = "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
        write(new_socket , hello , strlen(hello));
        close(new_socket);
    }
    return 0;
}
