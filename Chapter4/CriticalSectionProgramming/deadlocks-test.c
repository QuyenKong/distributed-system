#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lockA;
pthread_mutex_t lockB;

void* worker1(void* arg){
    pthread_mutex_lock(&lockA);
    printf("Thread 1 acquired lockA\n");
    sleep(1);
    pthread_mutex_lock(&lockB);
    printf("Thread 1 acquired lockB\n");
    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    return NULL;
}

void* worker2(void* arg){
    pthread_mutex_lock(&lockB);
    printf("Thread 2 acquired lockB\n");
    sleep(1);
    pthread_mutex_lock(&lockA);
    printf("Thread 2 acquired lockA\n");
    pthread_mutex_unlock(&lockA);
    pthread_mutex_unlock(&lockB);
    return NULL;
}

int main(){
    pthread_t t1,t2;
    pthread_mutex_init(&lockA, NULL);
    pthread_mutex_init(&lockB, NULL);

    pthread_create(&t1, NULL, worker1, NULL);
    pthread_create(&t2, NULL, worker2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lockA);
    pthread_mutex_destroy(&lockB);

    return 0;
}