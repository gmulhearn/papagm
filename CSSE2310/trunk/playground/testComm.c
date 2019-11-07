#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void connect_to(const char* port);
void host(void);

void print_quads(in_addr_t x) {
    char* bytes = (char*)&x;  
    for (int i=0; i<sizeof(x); ++i) {
        printf("%u.", (unsigned)bytes[i]);
    }
}

int main(int argc, char** argv) {
    if (argc<3) {
        fprintf(stderr, "Not enough args\n");
        return 2;
    }

    if (strcmp("connect", argv[1]) == 0) {
        printf("connecting to %s\n", argv[2]);
        connect_to(argv[2]);
    } else {
        host();
    }

    return 0;
}

void host(void) {
    struct addrinfo* ai = 0;
    struct addrinfo hints;
    memset(& hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_INET;
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
    char* msg="Go away!\n";
    while (conn_fd = accept(serv, 0, 0), conn_fd >= 0) {    // change 0, 0 to get info about other end
        int conn_fd2=dup(conn_fd);
        FILE* to = fdopen(conn_fd, "w");
        FILE* from = fdopen(conn_fd2, "r");
        fputs(msg, to);
        fflush(to);
        
        char buffer[80];
        fgets(buffer, 79, from);
        fprintf(stdout, "%s", buffer);
    }
    exit(0);
}


void connect_to(const char* port) {
    struct addrinfo* ai = 0;
    struct addrinfo hints;
    memset(& hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_INET;        // IPv6  for generic could use AF_UNSPEC
    hints.ai_socktype=SOCK_STREAM;
    int err;
    if (err=getaddrinfo("localhost", port, &hints, &ai)) {
        freeaddrinfo(ai);
        fprintf(stderr, "%s\n", gai_strerror(err));
        exit(1);   // could not work out the address
    }

    int fd=socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    if (connect(fd, (struct sockaddr*)ai->ai_addr, sizeof(struct sockaddr))) {
        perror("Connecting");
        exit(3);
    }
    // fd is now connected
    // we want separate streams (which we can close independently)
    
    int fd2=dup(fd);
    FILE* to=fdopen(fd, "w");
    FILE* from=fdopen(fd2, "r");
    
    fprintf(to, "Hello\n");
    fflush(to);
    
    char buffer[80];
    fgets(buffer, 79, from);
    fprintf(stdout, "%s", buffer);
}
