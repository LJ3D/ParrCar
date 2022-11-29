#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

#define SERV_PORT 52727
#define SERV_IP "192.168.159.111"

#define MAX_PACKET_SIZE 255

int main(){
    int sockfd;
    struct sockaddr_in servaddr;

    // Create socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERV_IP);

    while(1){
        char message[MAX_PACKET_SIZE] = "Hello world\n";
        // Flush the socket
        
        // Should "hello world" at the car
        int sent = sendto(sockfd, (const char*)message, strlen(message), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));
        std::cout << sent << std::endl;
        sleep(1);
    }

}