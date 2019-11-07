#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <signal.h>

#include <pthread.h>
#include <semaphore.h>
#include <netdb.h>

typedef enum {
    NORMAL_EXIT = 0,
    INVALID_NUM_ARGS = 1,
    INVALID_NAME = 2,
    INVALID_QUANTITY = 3
} Status;

typedef enum {
    CONNECT = 0,
    IM = 1,
    DELIVER = 2,
    WITHDRAW = 3,
    TRANSFER = 4,
    DEFER = 5,
    EXECUTE = 6,
    INVALID = 7
} Message;

//function for printing error status to stderr
//returns status
Status show_message(Status s) {
    const char* messages[] = {"",
            "Usage: 2310depot name {goods qty}\n",
            "Invalid name(s)\n",
            "Invalid quantity\n"};
    fputs(messages[s], stderr);
    return s;
}

typedef struct {
    char* name;
    int quantity;
} Resource;

typedef struct {
    FILE* to;
    FILE* from;
} Contact;

typedef struct {
    unsigned long port;
    char* name;
    Contact contact;
} Neighbour;

typedef struct {
    unsigned int key;
    char* instruction; //string starting after 'Defer:k:'
    bool executed;
} Defer;

typedef struct {
    char* name;
    char* port;

    Resource* resources;
    int numResources;

    Neighbour* neighbours;
    int numNeighbours;

    Defer* defers;
    int numDefers;

    int* ports;
    int numPorts;

    sem_t* guard;

    bool newConnectComplete;
} Depot;

typedef struct {
    Depot* depot;
    Contact contact; //used by connection()
    const char* port; //used by connect()
    int debug;
} ThreadPackage;

void process_transaction(Depot* depot, char* msg, int type);
Message process_msg(Depot* depot, char* msg, Contact contact, bool firstMsg);
void process_defer(Depot* depot, char* msg);
void process_execute(Depot* depot, char* msg, Contact contact);
void process_im(Depot* depot, char* msg, Contact contact);
void process_transfer(Depot* depot, char* msg);
void process_connect(Depot* depot, char* msg);


//global depot to be used by signal handler only
Depot* globalDepot;

//global mutex to be used globally
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/** NETWORKING **/

//function that continuously reads the connection's fd and processes
//those messages. Each thread other than the host thread runs this
void* connection(void* v) {
    
    ThreadPackage package = *((ThreadPackage*) v);
    Depot* depot = package.depot;
    Contact contact = package.contact;
    
    char msg[80]; //fix
    fgets(msg, 79, contact.from);
    bool firstMsg = true;

    while (!feof(contact.from)) {

        pthread_mutex_lock(&mutex);

        if (process_msg(depot, msg, contact, firstMsg) == CONNECT) {
            depot->newConnectComplete = false;
        }

        pthread_mutex_unlock(&mutex);

        fgets(msg, 79, contact.from);
        firstMsg = false;

    }
    
    return (void*)0;
}

//function for establishing a connection to the provided port
//creates a new connection thread once complete
void join(Depot* depot, const char* port) {

    struct addrinfo* ai = 0;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if (getaddrinfo("127.0.0.1", port, &hints, &ai)) {
        freeaddrinfo(ai);
        depot->newConnectComplete = true;
        return;
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(fd, (struct sockaddr*)ai->ai_addr, sizeof(struct sockaddr))) {
        depot->newConnectComplete = true;
        return; 
    }

    Contact contact;
    
    int fd2 = dup(fd);
    contact.to = fdopen(fd, "w");
    contact.from = fdopen(fd2, "r");

    char imMsg[420]; //fix
    sprintf(imMsg, "IM:%s:%s\n", depot->port, depot->name);
    fputs(imMsg, contact.to);
    fflush(contact.to);

    pthread_t tid;
    ThreadPackage package;
    package.depot = depot;
    
    package.contact = contact;
    
    depot->numPorts++;
    depot->ports = realloc(depot->ports, sizeof(int) * depot->numPorts);
    depot->ports[depot->numPorts - 1] = atoi(port);
    
    pthread_create(&tid, 0, connection, &package);
}

//function with creates a server, prints the port and then creates a 
//new connection thread for each new connection it receives.
void host(Depot* depot) {
    struct addrinfo* ai = 0;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;        // IPv6  for generic could use AF_UNSPEC
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  // Because we want to bind with it    
    if (getaddrinfo("127.0.0.1", 0, &hints, &ai)) { // no particular port
        freeaddrinfo(ai);
        return;   // could not work out the address fix
    } 
    int serv = socket(AF_INET, SOCK_STREAM, 0); // 0 == use default protocol
    if (bind(serv, (struct sockaddr*)ai->ai_addr, sizeof(struct sockaddr))) {
        return;
    } 
    struct sockaddr_in ad;
    memset(&ad, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(struct sockaddr_in);
    if (getsockname(serv, (struct sockaddr*)&ad, &len)) {
        return;
    }
    printf("%u\n", ntohs(ad.sin_port));
    fflush(stdout);
    char portString[420]; //fix
    sprintf(portString, "%u", ntohs(ad.sin_port)); //tell depot its port
    depot->port = portString; 
    if (listen(serv, 10)) { // allow up to 10 connection requests to queue
        return;
    } 
    int connFd;
    pthread_t tid;
    //main thread loop here
    while (connFd = accept(serv, 0, 0), connFd >= 0) { //change 0,0 for info
        ThreadPackage package;
        package.depot = depot;
	Contact contact;
        int fd2 = dup(connFd);
        contact.to = fdopen(connFd, "w");
        contact.from = fdopen(fd2, "r");
        char imMsg[420]; //fix
        //pthread_mutex_lock(&mutex);
        sprintf(imMsg, "IM:%s:%s\n", depot->port, depot->name);
        fputs(imMsg, contact.to);
        fflush(contact.to);

	package.contact = contact;
        pthread_create(&tid, 0, connection, &package);
    }
    return; //fix
}

/**    END     **/

//determines whether a string is a number
bool is_num(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] - '0' < 0 || string[i] - '0' > 9) {
            if (!(i == 0 && string[i] == '-')) { //unless negative
                return false;
            }
        }
    }
    if (strlen(string) == 0) {
        return false;
    }
    return true;
}

//determines whether a string is a valid name according to the spec
bool is_valid_name(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        char c = string[i];
        if (c == ' ' || c == '\n' || c == '\r' || c == ':') {
            return false;
        }
    }
    return true;
}

//starting from *pos, read string until it encounters the c character
//(or end of string)
//returns the string it read
char* read_until_char(char c, int* pos, char* msg) {

    int count = 0;
    char* string = malloc(sizeof(char) * 420); //fix buffer

    while (1) {
        char currCharacter = msg[*pos];
        if (currCharacter == '\0') {
            return string; //error handle here
        }
        if (currCharacter == c) {
            string[count] = '\0';
            return string;
        } else {
            string[count] = currCharacter;
            count++;
            *pos += 1;
        }
    }
}

//helper function for qsort that sorts resources by name
int compare_resources(const void* v1, const void* v2) {
    Resource r1 = *((Resource*) v1);
    Resource r2 = *((Resource*) v2);
    char* name1 = r1.name;
    char* name2 = r2.name;

    return strcmp(name1, name2);
}

//helper function for qsort that sorts neighbours by name
int compare_neighbours(const void* v1, const void* v2) {
    Neighbour n1 = *((Neighbour*) v1);
    Neighbour n2 = *((Neighbour*) v2);
    char* name1 = n1.name;
    char* name2 = n2.name;

    return strcmp(name1, name2);
}

//initial function for creating the main depot pointer from the starting args
//returns the depot pointer to be used by most functions
Depot* process_args(int argv, char** argc) {
    Depot* depot = malloc(sizeof(Depot));

    depot->numNeighbours = 0;
    depot->neighbours = malloc(sizeof(Neighbour) * 1); //will need realloc
    depot->numDefers = 0;
    depot->defers = malloc(sizeof(Defer) * 1); //realloced
    depot->numPorts = 0;
    depot->ports = malloc(sizeof(int) * 1); //realloced

    if (argv < 2 || (argv / 2) * 2 != argv) { //double check
        exit(show_message(INVALID_NUM_ARGS));
    } else {
        depot->numResources = (argv - 2) / 2;
        depot->resources = malloc(sizeof(Resource) * depot->numResources);
    }

    if (!is_valid_name(argc[1])) {
        exit(show_message(INVALID_NAME));
    } else {
        depot->name = argc[1];
    }

    for (int i = 2; i < argv; i += 2) { //checking names
        if (!is_valid_name(argc[i])) {
            exit(show_message(INVALID_NAME));
        } else {
            depot->resources[(i - 2) / 2].name = argc[i];
        }
    }
    for (int i = 3; i < argv; i += 2) { //checking quantities
        if (!is_num(argc[i]) || atoi(argc[i]) < 0) {
            exit(show_message(INVALID_QUANTITY));
        } else {
            depot->resources[(i - 3) / 2].quantity = atoi(argc[i]);
        }
    }

    qsort(depot->resources, depot->numResources,
            sizeof(Resource), compare_resources);

    return depot;
}

//function for processing a msg received by a connection thread
//returns the message type it received.
Message process_msg(Depot* depot, char* msg, Contact contact, bool firstMsg) {
    const char* connect = "Connect";
    const char* im = "IM";
    const char* deliver = "Deliver";
    const char* withdraw = "Withdraw";
    const char* transfer = "Transfer";
    const char* defer = "Defer";
    const char* execute = "Execute";

    if (!strncmp(msg, im, 2) && firstMsg) { //only run if first msg
        process_im(depot, msg, contact);
        return IM;
    } else if (firstMsg) { //firstMsg and not IM, so close connection
        fclose(contact.to);
        fclose(contact.from);
        return INVALID;
    } else if (!strncmp(msg, defer, 5)) {
        process_defer(depot, msg);
        return DEFER;
    } else if (!strncmp(msg, connect, 7)) {
        process_connect(depot, msg);
        return CONNECT;
    } else if (!strncmp(msg, deliver, 7)) {
        process_transaction(depot, msg, 0);
        return DELIVER;
    } else if (!strncmp(msg, execute, 7)) {
        process_execute(depot, msg, contact);
        return EXECUTE;
    } else if (!strncmp(msg, withdraw, 8)) {
        process_transaction(depot, msg, 1);
        return WITHDRAW;
    } else if (!strncmp(msg, transfer, 8)) {
        process_transfer(depot, msg);
        return TRANSFER;
    } else {
        return INVALID;
    }
    return INVALID;
}

//checks if the depot provided has the resource type given by the type string
//returns the index of depot->resources that matches the type
// or -1 if no match
int has_type(Depot* depot, char* type) {
    for (int i = 0; i < depot->numResources; i++) {
        if (strcmp(type, depot->resources[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

//used for processing delivering and withdrawing messages
//type 0 = deliver
//type 1 = withdraw
void process_transaction(Depot* depot, char* msg, int type) {
    int pos = 8 + type;
    char* q = read_until_char(':', &pos, msg);
    pos++;
    char* t = read_until_char('\0', &pos, msg);
    if (t[strlen(t) - 1] == '\n') {
        t[strlen(t) - 1] = '\0';
    }
    if (!is_valid_name(t)) {
        //printf(t);
        //fflush(stdout);
        return;
    }

    if (is_num(q)) {
        if (has_type(depot, t) != -1) {
            if (type == 0) {
                depot->resources[has_type(depot, t)].quantity += atoi(q);
            } else {
                depot->resources[has_type(depot, t)].quantity -= atoi(q);
            }
        } else { //new resource
            depot->numResources++;
            depot->resources = realloc(depot->resources,
                    sizeof(Resource) * depot->numResources);

            Resource newResource;
            newResource.name = t;
            if (type == 0) {
                newResource.quantity = 0 + atoi(q);
            } else {
                newResource.quantity = 0 - atoi(q);
            }
            depot->resources[depot->numResources - 1] = newResource;
            qsort(depot->resources, depot->numResources, 
                    sizeof(Resource), compare_resources);
        }
    } else {
        return; //ignore
    }
}

//function for processing an IM message. adds the new neighbour to the depot
//if its valid
void process_im(Depot* depot, char* msg, Contact contact) {
    //NOTE: TO DO: FIX: check if process IM has already been called
    depot->numNeighbours++;
    depot->neighbours = realloc(depot->neighbours, 
            sizeof(Neighbour) * depot->numNeighbours);

    int pos = 3;
    char* p = read_until_char(':', &pos, msg);

    pos++;
    char* name = read_until_char('\0', &pos, msg);
    if (name[strlen(name) - 1] == '\n') {
        name[strlen(name) - 1] = '\0';
    }
    
    if (!is_valid_name(name)) {
        //return;
    }

    Neighbour newNeighbour;
    newNeighbour.contact = contact;

    if (is_num(p)) {
        newNeighbour.port = atoi(p);
        newNeighbour.name = name;
        depot->neighbours[depot->numNeighbours - 1] = newNeighbour;
        qsort(depot->neighbours, depot->numNeighbours,
                sizeof(Neighbour), compare_neighbours);
    } else {
        return;
    }
}

//function for processing defer messages. adds them to the depots
//deferred messages list
void process_defer(Depot* depot, char* msg) {
    int pos = 6;
    char* k = read_until_char(':', &pos, msg);

    pos++;
    char* instruction = read_until_char('\0', &pos, msg);
    if (k[strlen(k) - 1] == '\n') {
        k[strlen(k) - 1] = '\0';
    }
    
    if (is_num(k)) {
        Defer defer;
        defer.instruction = instruction;
        defer.executed = false;
        defer.key = atoi(k);

        depot->numDefers++;
        depot->defers = realloc(depot->defers, 
                sizeof(Defer) * depot->numDefers);
        depot->defers[depot->numDefers - 1] = defer;
    }
}

//function for processing execution messages. cycles through the depots
//deffered messages and executes those with the identical key
void process_execute(Depot* depot, char* msg, Contact contact) {
    int pos = 8;
    char* k = read_until_char('\0', &pos, msg);
    if (k[strlen(k) - 1] == '\n') {
        k[strlen(k) - 1] = '\0';
    }
    if (!is_num(k)) {
        return;
    }

    for (int i = 0; i < depot->numDefers; i++) {
        if (depot->defers[i].key == atoi(k) && !depot->defers[i].executed) {
            process_msg(depot, depot->defers[i].instruction, contact, false);
            depot->defers[i].executed = true;
        }
    }
}

//function for processing a transfer message. Sends the corresponding
//transaction message to itself and to the other depot
void process_transfer(Depot* depot, char* msg) {
    int pos = 9;
    char* q = read_until_char(':', &pos, msg);

    pos++;
    char* t = read_until_char(':', &pos, msg);

    pos++;
    char* dest = read_until_char('\0', &pos, msg);
    if (dest[strlen(dest) - 1] == '\n') {
        dest[strlen(dest) - 1] = '\0';
    }

    char withdrawMsg[420]; //fix
    sprintf(withdrawMsg, "Withdraw:%s:%s\n", q, t);
    //process_transaction(depot, withdrawMsg, 1);

    char deliverMsg[420]; //fix
    sprintf(deliverMsg, "Deliver:%s:%s\n", q, t);

    for (int i = 0; i < depot->numNeighbours; i++) {
        if (strcmp(depot->neighbours[i].name, dest) == 0) {
            process_transaction(depot, withdrawMsg, 1); //withdraw from this
            fputs(deliverMsg, depot->neighbours[i].contact.to); //send out
            fflush(depot->neighbours[i].contact.to);
            break;
        }
    }
}

//function for determining whether the provided port number has already 
//been connected to by this depot
bool is_connected(Depot* depot, int port) {
    for (int i = 0; i < depot->numPorts; i++) {
        if (depot->ports[i] == port) {
            return true;
        }
    }
    return false;
}

//function for processing a connect message. If the message is valid,
//join will be called which will then attempt to connect to the provided port
void process_connect(Depot* depot, char* msg) {
    int pos = 8;
    char* p = read_until_char('\0', &pos, msg);
    if (p[strlen(p) - 1] == '\n') {
        p[strlen(p) - 1] = '\0';
    }
     
    if (!is_num(p)) {
        return;
    }
    if (!is_connected(depot, atoi(p))) {
        join(depot, p);
    }
}

//function for converting the depots status to string in accordance with the 
//specs format. - To be used by hang up signal handler
char* hangup_msg(Depot* depot) {
    char* msg = malloc(sizeof(char) * 420); //fix
    strcpy(msg, "Goods:\n");
    for (int i = 0; i < depot->numResources; i++) {
        char resourceS[69]; //fix
        Resource resource = depot->resources[i];
        if (resource.quantity != 0) {
            sprintf(resourceS, "%s %d\n", resource.name, resource.quantity);
            strcat(msg, resourceS);
        }
    }
    strcat(msg, "Neighbours:\n");
    for (int i = 0; i < depot->numNeighbours; i++) {
        char neighbourS[69]; //fix
        Neighbour neighbour = depot->neighbours[i];

        sprintf(neighbourS, "%s\n", neighbour.name);
        strcat(msg, neighbourS);
    }

    return msg;
}

//function for handling hang up signal
void handler(int s) {
    pthread_mutex_lock(&mutex);
    printf(hangup_msg(globalDepot));
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
}


//main
int main(int argc, char** argv) {

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART; //fix
    sigaction(SIGHUP, &sa, 0);

    Depot* depot = process_args(argc, argv);
    
    globalDepot = depot;

    host(depot);

    return 0;
}




