#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

void* connection(void* v) {
    int fd = *(int*) v;
    int fd2 = dup(fd);
    FILE* to = fdopen(fd, "w");
    FILE* from = fdopen(fd2, "r");
    
    while (1) { 
    //    fprintf(to, "Hello\n");
    //    fflush(to);
    //    sleep(2);

        char buffer[80];
        fgets(buffer, 79, from);
        fprintf(stdout, "%s", buffer);
    }

    return (void*)0;
}


void host(void) {
    struct addrinfo* ai = 0;
    struct addrinfo hints;
    memset(& hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_INET;        // IPv6  for generic could use AF_UNSPEC
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;  // Because we want to bind with it    
    int err;
    if (err=getaddrinfo("localhost", 0, &hints, &ai)) { // no particular port
        freeaddrinfo(ai);
        fprintf(stderr, "%s\n", gai_strerror(err));
        exit(1);   // could not work out the address
    }
    
        // create a socket and bind it to a port
    int serv = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    if (bind(serv, (struct sockaddr*)ai->ai_addr, sizeof(struct sockaddr))) {
        perror("Binding");
        exit(3);
    }
    
        // Which port did we get?
    struct sockaddr_in ad;
    memset(&ad, 0, sizeof(struct sockaddr_in));
    socklen_t len=sizeof(struct sockaddr_in);
    if (getsockname(serv, (struct sockaddr*)&ad, &len)) {
        perror("sockname");
        exit(4);
    }
    printf("%u\n", ntohs(ad.sin_port));
    
    if (listen(serv, 10)) {     // allow up to 10 connection requests to queue
        perror("Listen");
        exit(4);
    }
    
    int conn_fd;
    pthread_t tid;

    while (conn_fd = accept(serv, 0, 0), conn_fd >= 0) {    // change 0, 0 to get info about other end
        pthread_create(&tid, 0, connection, &conn_fd);
        printf("tick\n");
    }
}

int main(int argc, char** argv) {
    host();
}
