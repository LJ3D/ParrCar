#include <iostream>
#include <string>
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <morph/Visual.h>
#include <morph/ColourMap.h>
#include <morph/GraphVisual.h>
#include <morph/Scale.h>
#include <morph/Vector.h>
#include <morph/vVector.h>

#define SERV_PORT 52727 // The port the server is listening on

int main(){
    // Morph stuff
    morph::Visual v(1800, 600, "Graph", {0, 0}, {.1,.1,.1}, 2.0f, 0.01f);
    v.zNear = 0.001;
    v.backgroundWhite(); // Sets the background colour to white
    v.lightingEffects(); // Enables lighting (shadows etc) to 3D objects
    morph::GraphVisual<float>* gv = new morph::GraphVisual<float>(v.shaderprog, v.tshaderprog, {0, 0, 0});
    morph::GraphVisual<float>* gv2 = new morph::GraphVisual<float>(v.shaderprog, v.tshaderprog, {0, 1.2, 0});
    morph::GraphVisual<float>* gv3 = new morph::GraphVisual<float>(v.shaderprog, v.tshaderprog, {0, 2.4, 0});
    gv->setsize (4, 1);
    gv2->setsize (4, 1);
    gv3->setsize (4, 1);
    int limit = 2048;
    gv->setlimits(-1, limit, -2000, 0);
    gv2->setlimits(-1, limit, -500, 1500);
    gv3->setlimits(-1, limit, -5000, -4000);
    gv->policy = morph::stylepolicy::lines; // markers, lines, both, allcolour
    gv2->policy = morph::stylepolicy::lines; // markers, lines, both, allcolour
    gv3->policy = morph::stylepolicy::lines; // markers, lines, both, allcolour
    gv->prepdata("compassX", morph::axisside::left);
    gv2->prepdata("compassY", morph::axisside::left);
    gv3->prepdata("compassZ", morph::axisside::left);
    gv->finalize();
    gv2->finalize();
    gv3->finalize();
    v.addVisualModel(static_cast<morph::VisualModel*>(gv)); // Add the GraphVisual (as a VisualModel*)
    v.addVisualModel(static_cast<morph::VisualModel*>(gv2)); // Add the GraphVisual (as a VisualModel*)
    v.addVisualModel(static_cast<morph::VisualModel*>(gv3)); // Add the GraphVisual (as a VisualModel*)

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
    if((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    // Set options on new socket
    if(setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // The main control loop:
    int i = 0;
    while(v.readyToFinish == false){
        glfwWaitEventsTimeout(0.018);
        char buffer[32] = {0};

        send(new_socket, "2", 1, 0); // 2 = X, 3 = Y, 4 = Z
        memset(buffer, 0, 32);
        read(new_socket, buffer, 32);
        int x = atoi(buffer);
        gv->append(i, x, 0);

        send(new_socket, "3", 1, 0); // 2 = X, 3 = Y, 4 = Z
        memset(buffer, 0, 32);
        read(new_socket, buffer, 32);
        int y = atoi(buffer);
        gv2->append(i, y, 0);

        send(new_socket, "4", 1, 0); // 2 = X, 3 = Y, 4 = Z
        memset(buffer, 0, 32);
        read(new_socket, buffer, 32);
        int z = atoi(buffer);
        gv3->append(i, z, 0);

        if(i++ > limit){ // Reset the graphs if they get too big
            i = 0;
            gv->clear();
            gv2->clear();
            gv3->clear();
        }

        v.render(); // Render the graph
    }
    close(fd); // Close the socket
    return 0;
}
