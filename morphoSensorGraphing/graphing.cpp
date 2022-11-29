#include <iostream>
#include <string>
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 

#include "communicationUtils.h"

#define SERV_PORT 52727 // The port the server is listening on

int main(){
    // Socket stuff
    int fd; // The file descriptor for the socket
    int opt; // Used for setting socket options
    struct sockaddr_in address; // The address of the server

    // Create the socket:
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set the socket options:
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Set the address:
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    address.sin_family = AF_INET;
    address.sin_port = htons(SERV_PORT); // Listen on the specified port

    // Bind the socket to the address:
    if(bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections:
    if(listen(fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Wait for and accept a new connection:
    int addrlen = sizeof(address);
    int new_socket;
    std::cout << "Waiting for a connection..." << std::endl;
    if((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connection accepted" << std::endl;
    // The main control loop:
    while(true){
        std::cout << readUntil(new_socket, '\n');
    }
    close(fd); // Close the socket
    return 0;
}
