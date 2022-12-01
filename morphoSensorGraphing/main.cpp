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
    gv->setsize(4, 1);
    gv2->setsize(4, 1);
    int limit = 256;
    gv->setlimits(-1, limit, 0, 360);
    gv2->setlimits(-1, limit, 0, 250);
    gv->policy = morph::stylepolicy::lines; // markers, lines, both, allcolour
    gv2->policy = morph::stylepolicy::lines;
    gv->prepdata("azimuth", morph::axisside::left);
    gv2->prepdata("distance", morph::axisside::left);
    gv->finalize(); // Finalize the graph (whatever that means)
    gv2->finalize();
    v.addVisualModel(static_cast<morph::VisualModel*>(gv)); // Add the GraphVisual (as a VisualModel*)
    v.addVisualModel(static_cast<morph::VisualModel*>(gv2));

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
    address.sin_addr.s_addr = inet_addr("192.168.1.1");
    address.sin_family = AF_INET;
    address.sin_port = htons(SERV_PORT); // Connect to the server on port SERV_PORT

    // Connect to the server:
    if(connect(fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // The main control loop:
    int i = 0;
    while(v.readyToFinish == false){
        glfwWaitEventsTimeout(0.018);
        char buffer[32] = {0};

        send(fd, "2", 1, 0); // 2 = X, 3 = Y, 4 = Z
        memset(buffer, 0, 32);
        read(fd, buffer, 32);
        int x = atoi(buffer);
        gv->append(i, x, 0);

        send(fd, "1", 1, 0);
        memset(buffer, 0, 32);
        read(fd, buffer, 32);
        int dist = atoi(buffer);
        gv2->append(i, dist, 0);

        if(glfwGetKey(v.window, GLFW_KEY_UP)){
            send(fd, "w", 1, 0);
            // Receive reply:
            memset(buffer, 0, 32);
            read(fd, buffer, 32);
            printf("Received: %s\n", buffer);
        }
        if(glfwGetKey(v.window, GLFW_KEY_LEFT) == GLFW_PRESS){
            send(fd, "a", 1, 0);
            // Receive reply:
            memset(buffer, 0, 32);
            read(fd, buffer, 32);
            printf("Received: %s\n", buffer);
        }
        if(glfwGetKey(v.window, GLFW_KEY_DOWN) == GLFW_PRESS){
            send(fd, "s", 1, 0);
            // Receive reply:
            memset(buffer, 0, 32);
            read(fd, buffer, 32);
            printf("Received: %s\n", buffer);
        }
        if(glfwGetKey(v.window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            send(fd, "d", 1, 0);
            // Receive reply:
            memset(buffer, 0, 32);
            read(fd, buffer, 32);
            printf("Received: %s\n", buffer);
        }
        if(glfwGetKey(v.window, GLFW_KEY_Q) == GLFW_PRESS){
            send(fd, "q", 1, 0);
            // Receive reply:
            memset(buffer, 0, 32);
            read(fd, buffer, 32);
            printf("Received: %s\n", buffer);
        }

        if(i++ > limit){ // Reset the graphs if they get too big
            i = 0;
            gv->clear();
            gv2->clear();
        }

        v.render(); // Render the graph
    }
    close(fd); // Close the socket
    return 0;
}
