/*

Not a proper library, just some functions that are handy for communicating with the car from the computer.

*/

#include <iostream>
#include <string>
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 


// Read until terminator char or end of buffer
// Returns std::string
std::string readUntil(int socket, char terminator){
    char charBuffer = 0;
    std::string str = "";
    while(charBuffer != terminator){
        int valread = read(socket, &charBuffer, 1);
        if(valread < 0){
            perror("read");
            exit(EXIT_FAILURE);
        }
        str += charBuffer;
    }
    return str;
}
