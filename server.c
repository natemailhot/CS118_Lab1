#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/wait.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFSIZE 30000

void header(int socket, char *file, int size, int error)
{
    char msg[1000];
    int position = 0;

    //Check the status
    char *status;
    if (!error)
        status = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: ";
    else if (error)
        status = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: ";
    memcpy(msg, status, strlen(status));
    position += strlen(status);

    //Content type
    char *type = strrchr(file, '.');
    char *type_def;
    if (type!= NULL){
        if (strcasecmp(type,".jpeg") == 0)
            type_def = "image/jpeg\r\n";
        else if(strcasecmp(type,".jpg") == 0)
            type_def = "image/jpg\r\n";
        else if(strcasecmp(type,".png") == 0)
            type_def = "image/png\r\n";
        else if(strcasecmp(type,".gif") == 0)
            type_def = "image/gif\r\n";
        else if(strcasecmp(type,".txt") == 0)
            type_def = "text/plain\r\n";
        else
            type_def = "text/html\r\n";
        memcpy(msg+position, type_def, strlen(type_def));
        position += strlen(type_def);
    }
    else{ //Need to search the database for file 
	        	
    }

    //Content length
    char cl[30] = "Content-Length: ";
    char len[16];
    sprintf(len, "%d", size);
    strcat(cl, len);
    strcat(cl, "\r\n");
    memcpy(msg+position, cl, strlen(cl));
    position += strlen(cl);
    memcpy(msg+position, "\r\n\0", 3);

    //send and print completed response message
    write(socket, msg, strlen(msg));
    printf("HTTP Response Message:\n%s\n", msg);
}

void handle_socket(int socket)
{
    //Reading
    char buffer[BUFFSIZE] = {0};  
    int read_bytes = read(socket , buffer, BUFFSIZE);
    if(read_bytes < 0){
	perror("Error: Couldn't read from socket");
	exit(EXIT_FAILURE);
    }
     
    //Writing
    char *file;

    //Obtain the file name
    file = strtok(buffer, " ");
    file = strtok(NULL, " ");
    file++;

    //Adding functionality to not need extension
    char *type = strrchr(file, '.');
    
    char* e404 = "<HTML><HEAD><TITLE>404 not found error</TITLE></HEAD><BODY><H1>404 not found error</H1></BODY></HTML>";
    //Send a response
    if(*file == '\0'){
        header(socket, file, 0, 1);
	write(socket, e404, strlen(e404));
        perror("Error: File doesn't exist");
	return;
    }

    const char *names [7] = {".html", ".htm", ".txt", ".jpeg", ".jpg", ".png"};

    FILE *f = fopen(file, "r");
    if (!f){
	int found = 0;
	if(type == NULL){ //there is no . in the file name so we must search for it
	    for(int i = 0; i < 7; i++){
		char temp[100];
		strcpy(temp, file);
		strcat(temp, names[i]);
		printf(temp, strlen(temp));
		printf("\n");
		f = fopen(temp, "r");
		if(!f){
		    continue;
		}
		found = 1;
		file = temp;
		break;
	    } 
	}
	if(!found){
	    header(socket, file, 0, 1);
	    write(socket, e404, strlen(e404));
       	    perror("Error: Opening file");
            return;
	}
    }

    if (!fseek(f, 0, SEEK_END)){
        long size = ftell(f);

        if (size < 0){
            header(socket, file, 0, 1);
	    write(socket, e404, strlen(e404));
            perror("Error: File length");
	    return;
        }

        char *file_buff = malloc(sizeof(char) * (size + 1));
        fseek(f, 0, SEEK_SET);

        size_t bytes = fread(file_buff, sizeof(char), size, f);
        file_buff[bytes] = '\0';
        header(socket, file, bytes, 0);
        write(socket, file_buff, bytes);
        free(file_buff);
    }
    fclose(f);
}

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
    struct sockaddr_in client_address;
    int addrlen = sizeof(client_address);

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

    printf("Server Starting\n");

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

	int pid = fork();
        if(pid < 0) {
            perror("Error: Could not fork");
            exit(EXIT_FAILURE);
        } else if(pid > 0){
            close(new_socket);
        } else {
            close(server_fd);
	    handle_socket(new_socket);
	    exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
