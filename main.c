#include <signal.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#define MAX 1024 
#define PORT 8080 
#define SA struct sockaddr 
  
// Function designed for chat between client and server. 
void func(int connfd) 
{ 
    char in_buff[MAX]; 
    bzero(in_buff, MAX); 

    char out_buff[MAX] = "HTTP/1.1 200 OK\n"
        "Date: Mon, 27 Jul 2009 12:28:53 GMT\n"
        "Server: Apache\n"
        "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n"
        "ETag: \"34aa387-d-1568eb00\"\n"
        "Accept-Ranges: bytes\n"
        "Content-Length: 51\n"
        "Vary: Accept-Encoding\n"
        "Content-Type: text/plain\n"
        "\n"
        "Hello World! My content includes a trailing CRLF.\n";


    int n = 0; 

    // read the message from client and copy it in buffer 
    read(connfd, in_buff, sizeof(in_buff)); 

    // print buffer which contains the client contents 
    printf("From client: %s\n", in_buff); 

    // copy server message in the buffer 
    // while ((buff[n++] = getchar()) != '\n') 
    //     ; 

    // and send that buffer to client 
    write(connfd, out_buff, sizeof(out_buff)); 

    // if msg contains "Exit" then server exit and chat ended. 
    // if (strncmp("exit", buff, 4) == 0) { 
    //     printf("Server Exit...\n"); 
    //     break; 
    // } 
} 

int sockfd;

void handle_exit(int code) {
    printf("Gracefully shutting down...\n");
    close(sockfd); 
    exit(0);
}
  
// Driver function 
int main() 
{ 
    int connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    signal(SIGINT, handle_exit);

    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    while(1) {
        // Accept the data packet from client and verification 
        connfd = accept(sockfd, (SA*)&cli, (unsigned int*) &len); 
        if (connfd < 0) { 
            printf("server accept failed...\n"); 
            continue;
        } 
        else printf("server accept the client...\n"); 
      
        // Function for chatting between client and server 
        func(connfd); 
      
        // After chatting close the socket 
    }
}
