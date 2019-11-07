#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    double (*f)(double);
    double a;
    double b;
    int N;
    int kLow;
    int kHigh;
} ThreadPackage;


double function(double x) {
    
    double y = x*x*x;

    return y;
}

void* part_sum(void* v) {
    ThreadPackage* p = ((ThreadPackage*) v);
    double* sum = malloc(sizeof(double));

    for (int k = p->kLow; k < p->kHigh; k++) {
        double x = p->a + k*(p->b - p->a)/p->N;
        
        *sum += (p->f)(x);
    }
    //*sum = 0;
    //fprintf(stderr, "%f\n", *sum);

    return sum;

}

double integrate(double (*f)(double), double a, double b, int N, int T) {
    double total = 0;
    
    pthread_t tids[T];
    
    for (int i = 0; i < T; i++) {
        ThreadPackage* p = malloc(sizeof(ThreadPackage));
        p->a = a;
        p->b = b;
        p->N = N;
        p->kLow = N/T * i; //inclusive
        p->kHigh = N/T * (i + 1); //exclusive
        p->f = f;

        pthread_create(&tids[i], 0, part_sum, (void*) p);
        //sleep(1);
    }

    //sum them up

    for (int i = 0; i < T; i++) {
        void* retV;
        pthread_join(tids[i], &retV);
        
        total += *((double*) retV);
    }



    return ((b - a)/N * total);
}



int main(int argc, char** argv) {

    printf("%f\n", integrate(&function, atoi(argv[1]),
                atoi(argv[2]), atoi(argv[3]), atoi(argv[4])));

    return 0;
}
