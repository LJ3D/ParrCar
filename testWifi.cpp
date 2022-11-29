// Need to clean up these includes lol
// Not using a bunch of these probably
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ncurses.h>

#define SERV_PORT 52727

/*

    Creates a server that listens for a connection on port 52727
    When a connection is made, it will send user input to the client

*/

int main(){
    initscr();
    cbreak();
    noecho();
    WINDOW * win = newwin(0, 0, 0, 0);
    refresh();
    wrefresh(win);
    
    // Socket stuff
    int fd;
    char buffer[1024];
    int opt;

    struct sockaddr_in address;

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(SERV_PORT);
    if(bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if(listen(fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int addrlen = sizeof(address);
    int new_socket;
    mvwprintw(win, 2, 0, "Waiting for connection...");
    refresh();
    wrefresh(win);
    if((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    mvwprintw(win, 2, 0, "Connected to client");
    refresh();
    wrefresh(win);
    while(true){
        mvwprintw(win, 0, 0, "Press a key to send to client (press e to exit)");
        refresh();
        wrefresh(win);
        int c = getch();
        if(c == 'e'){
            break;
        }
        int bytes_sent = -1;
        if(c != ERR){
            std::string s(1, c);
            s += '\n';
            bytes_sent = send(new_socket, (s.c_str()), s.length(), 0);
        }
        mvwprintw(win, 1, 0, "Sent %d bytes", bytes_sent);
        refresh();
        wrefresh(win);
    }
    endwin();
    close(fd);
    return 0;
}
