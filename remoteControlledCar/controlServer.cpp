#include <ncurses.h> // Using ncurses mainly for getch() lol
#include <string>
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 

#define SERV_PORT 52727 // The port the server is listening on

int main(){
    // Initialise ncurses:
    initscr();
    cbreak();
    noecho();
    WINDOW * win = newwin(0, 0, 0, 0);
    refresh();
    wrefresh(win);
    
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
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
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
    mvwprintw(win, 2, 0, "Waiting for connection...");
    wrefresh(win);
    if((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    mvwprintw(win, 2, 0, "Connected to client        ");
    wrefresh(win);
    // The main control loop:
    int i = 0;
    while(true){
        mvwprintw(win, 0, 0, "Press a key to send to client (press e to exit)");
        wrefresh(win);
        int c = getch(); // Get a single char of input
        if(c == 'e'){
            break; // Exit the main loop if the user wants to
        }
        int bytes_sent = -1;
        if(c != ERR){ // As long as getch didnt give us an error, send c to the car
            std::string s(1, c);
            bytes_sent = send(new_socket, (s.c_str()), s.length(), 0);
        }
        mvwprintw(win, 1, 0, "Sent command number %d to client", i);
        i++;
        // Receive a message from the client:
        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, 1024);
        mvwprintw(win, 3, 0, "Received message from client: %s           ", buffer);
        wrefresh(win);
    }
    endwin(); // End ncurses
    close(fd); // Close the socket
    return 0;
}
