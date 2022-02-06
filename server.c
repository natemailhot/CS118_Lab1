#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>


#define PORT 8080
#define BUFFSIZE 30000

int main(int argc, char const *argv[])
{
    if(argc != 3){
	perror("Error: More than 3 args");
	exit(EXIT_SUCCESS);
    }

    if(chdir(argv[2]) == -1){
        perror("Error: Unable to change document directory");
	exit(EXIT_SUCCESS);
    }

    int server_fd;
    //Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error: Could not create socket file descriptor");
        exit(EXIT_FAILURE);
    }

    //Use old port number
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
        perror("Error: Failed to use old port number");
        exit(EXIT_SUCCESS);
    }

    struct sockaddr_in address;
    struct sockaddr_in client_address;
    int addrlen = sizeof(client_address);

    //old child process
    signal(SIGCLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

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

    printf("Server Starting");

    //Accept, read, and write
    //Loop to accept connections
    int new_socket;
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen))<0)
        {
            perror("Error: Could not accept connection");
            exit(EXIT_FAILURE);
        }

	/*
        //Reading
        char buffer[BUFFSIZE] = {0};
        int read_bytes = read(new_socket , buffer, BUFFSIZE);
        //printf("%s\n",buffer );

        //Writing
        char *hello = "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
        write(new_socket , hello , strlen(hello));
        close(new_socket);
	*/

	pid_t pid = fork();
        if(pid < 0) {
            perror("Error: Could not fork");
            exit(EXIT_FAILURE);
        } else {
            if(pid > 0) {
                close(new_socket);
            } else {
                close(server_fd);
                //create server
                //server(new_socket);
        	char *hello = "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
        	write(new_socket , hello , strlen(hello));
        	close(new_socket);
            }
        }
    }
    return 0;
}
