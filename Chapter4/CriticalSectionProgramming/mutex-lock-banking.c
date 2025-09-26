#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int Balance = 1000;         
pthread_mutex_t lock;       

void* deposit(void* arg){
    int num_trans = *((int*) arg);
    for(int i=0; i<num_trans; i++){
        pthread_mutex_lock(&lock);
        Balance = Balance + 1;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* withdraw(void* arg){
    int num_trans = *((int*) arg);
    for(int i=0; i<num_trans; i++){
        pthread_mutex_lock(&lock);
        Balance = Balance - 1;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if(argc < 4){
        printf("Usage: %s <NUM_DEPOSIT_THREADS> <NUM_WITHDRAW_THREADS> <NUM_TRANS>\n", argv[0]);
        return 1;
    }

    int NUM_DEPOSIT = atoi(argv[1]);
    int NUM_WITHDRAW = atoi(argv[2]);
    int NUM_TRANS = atoi(argv[3]);
    int NUM_THREADS = NUM_DEPOSIT + NUM_WITHDRAW;

    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);

    // Tạo luồng nạp tiền
    for(int i=0; i<NUM_DEPOSIT; i++)
        pthread_create(&threads[i], NULL, deposit, &NUM_TRANS);

    // Tạo luồng rút tiền
    for(int i=NUM_DEPOSIT; i<NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, withdraw, &NUM_TRANS);

    for(int i=0; i<NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);

    int credits = NUM_DEPOSIT * NUM_TRANS;
    int debits  = NUM_WITHDRAW * NUM_TRANS;
    int expected = 1000 + credits - debits;

    printf("Deposit Threads: %d, Withdraw Threads: %d, Trans/Thread = %d\n",
           NUM_DEPOSIT, NUM_WITHDRAW, NUM_TRANS);

    printf("Final Balance   = %d\n", Balance);
    printf("Expected Balance= %d\n", expected);

    return 0;
}