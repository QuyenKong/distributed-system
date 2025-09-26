#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N_ACCOUNTS 1000
#define INIT_BALANCE 1000
#define NUM_THREADS 8
#define NUM_TRANS 100000

int accounts[N_ACCOUNTS];
pthread_mutex_t coarse_lock;
pthread_mutex_t fine_locks[N_ACCOUNTS];

double now(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec*1e-9;
}

// coarse worker
void* coarse_worker(void *arg){
    for (int i=0; i<NUM_TRANS; i++){
        int from = rand() % N_ACCOUNTS;
        int to   = rand() % N_ACCOUNTS;
        int amount = rand() % 100;

        pthread_mutex_lock(&coarse_lock);
        if (accounts[from] >= amount) {
            accounts[from] -= amount;
            accounts[to] += amount;
        }
        pthread_mutex_unlock(&coarse_lock);
    }
    return NULL;
}

// fine worker
void* fine_worker(void *arg){
    for (int i=0; i<NUM_TRANS; i++){
        int from = rand() % N_ACCOUNTS;
        int to   = rand() % N_ACCOUNTS;
        int amount = rand() % 100;

        int first = from < to ? from : to;
        int second = from < to ? to : from;

        pthread_mutex_lock(&fine_locks[first]);
        pthread_mutex_lock(&fine_locks[second]);

        if (accounts[from] >= amount) {
            accounts[from] -= amount;
            accounts[to] += amount;
        }

        pthread_mutex_unlock(&fine_locks[second]);
        pthread_mutex_unlock(&fine_locks[first]);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    double start, end;

    // ---------- Coarse Lock ----------
    for (int i=0; i<N_ACCOUNTS; i++)
        accounts[i] = INIT_BALANCE;
    pthread_mutex_init(&coarse_lock, NULL);

    start = now();
    for (int i=0; i<NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, coarse_worker, NULL);
    for (int i=0; i<NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    end = now();

    printf("Coarse Locking: %.6f sec\n", end - start);
    pthread_mutex_destroy(&coarse_lock);

    // ---------- Fine Lock ----------
    for (int i=0; i<N_ACCOUNTS; i++){
        accounts[i] = INIT_BALANCE;
        pthread_mutex_init(&fine_locks[i], NULL);
    }
    // start = now();
    // for (int i=0; i<NUM_THREADS; i++)
    //     pthread_create(&threads[i], NULL, fine_worker, NULL);
    // for (int i=0; i<NUM_THREADS; i++)
    //     pthread_join(threads[i], NULL);
    // end = now();

    // printf("Fine Locking:   %.6f sec\n", end - start);

    // for (int i=0; i<N_ACCOUNTS; i++)
    //     pthread_mutex_destroy(&fine_locks[i]);

    return 0;
}