#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void* exec(void* v) {
    char* string = (char*) v;
    printf(string);
    fflush(stdout);
    return (void*) 0;
}

char* read_stdin(void) {
    char* string = malloc(sizeof(char) * 420); //bruh
    int count = 0;

    while (1) {
        char currChar = fgetc(stdin);
        if (currChar == '\n') {
            string[count] = '\0';
            return string;
        } else {
            string[count] = currChar;
            count++;
        }
    }

    return string;
}


int main(int argc, char** argv) {
    pthread_t tid;
    
    while (1) {
        char* msg = read_stdin();
        pthread_create(&tid, 0, exec, msg);
    }
    pthread_exit((void*)0);
}
