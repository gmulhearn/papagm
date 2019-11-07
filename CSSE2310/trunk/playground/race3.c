#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define LOOPS 200000
#define PCOUNT 6

typedef struct {
    int numPorts;
    int otherNum;
} Depot;

struct Param {
    Depot* depot;
    pthread_t tid;
    sem_t* guard;
};

void init_lock(sem_t* l) {
    sem_init(l, 0, 1);
}

void  take_lock(sem_t* l) {
    sem_wait(l);
}

void release_lock(sem_t* l) {
    sem_post(l);
}

void* fn(void* v) {
    struct Param* p = (struct Param*)v;
    p->tid=pthread_self();
    
    int num = 0;

    for (int i=0; i < LOOPS; ++i) {

	take_lock(p->guard);
        (p->depot->numPorts)++;
	release_lock(p->guard);

        num++;

    }


    fprintf(stderr, "%d\n", num);
    return 0;
}


int main(int argc, char** argv) {
    Depot* depot = malloc(sizeof(Depot));
    depot->numPorts = 0;
    sem_t lock;
    init_lock(&lock);
    struct Param pars[PCOUNT];
    pthread_t tids[PCOUNT];
    for (int i = 0; i < 6; ++i) {
	pars[i].depot = depot;
        pars[i].guard = &lock;
        pthread_create(&(tids[i]), 0, fn, pars+i);
    }
    for (int i = 0;i < 6; ++i) {
	void* v;
        pthread_join(tids[i], &v);
    }
    sem_destroy(&lock);
    printf("Expected %d got %d\n", LOOPS*PCOUNT, depot->numPorts);
    printf("Lost %d updates\n", LOOPS*PCOUNT-depot->numPorts);
    return 0;
}
