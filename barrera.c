#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int N;
    int stage;
} Barrier_t;

Barrier_t barrier;

void barrier_init(Barrier_t* barrier, int nThreads) {
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    barrier->N = nThreads;
    barrier->count = 0;
    barrier->stage = 0;
}

void barrier_wait(Barrier_t* barrier) {
    pthread_mutex_lock(&barrier->mutex);

    int currentStage = barrier->stage;
    barrier->count++;

    if (barrier->count == barrier->N) {
        barrier->count = 0;
        barrier->stage++;
        pthread_cond_broadcast(&barrier->cond);
    } else {
        while (barrier->stage == currentStage) {
            pthread_cond_wait(&barrier->cond, &barrier->mutex);
        }
    }
    
    pthread_mutex_unlock(&barrier->mutex);
}


void barrier_set_N(Barrier_t* barrier, int N) {
    pthread_mutex_lock(&barrier->mutex);

    while (barrier->count != 0) {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }
    
    barrier->N = N;

    pthread_mutex_unlock(&barrier->mutex);
}

void barrier_destroy(Barrier_t* barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
}

void* work(void* arg) {
    long etapa = (long)arg;

    printf("[tid]: %d esperando en etapa %ld\n", gettid(), etapa);
    sleep(rand() % 3);

    barrier_wait(&barrier);

    printf("[tid]: %d paso barrera en etapa %ld\n", gettid(), etapa);
    
    return NULL;
}

// ./barrera [N:int] [E:int]
int main(int argc, char *argv[]) {
    if (argc != 1 && argc != 3) {
        printf("Error: argumentosinsuficientes\n");
        printf("Uso: ./barrera [N:int] [E:int]\n");
        return -1;
    }

    int N;
    int E;
    if (argc == 1) {
        N = 5;
        E = 4;
    } else {
        N = atoi(argv[1]);
        E = atoi(argv[2]);
    }
    pthread_t threads[N];
    
    barrier_init(&barrier, N);

    for (long etapa = 0; etapa < E; etapa++) {
        for (int i = 0; i < N; i++) {
            pthread_create(&threads[i], NULL, work, (void*)etapa);
        }

        for (int i = 0; i < N; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    // // First round -----------------------------
    // const int N1 = 5;
    // pthread_t threads[N1];

    // barrier_init(&barrier, N1);

    // for (int i = 0; i < N1; i++) {
    //     pthread_create(&threads[i], NULL, work, NULL);
    //     sleep(1);
    // }

    // for (int i = 0; i < N1; i++) {
    //     pthread_join(threads[i], NULL);
    // }

    // printf("All the works have been done. Round 1\n");
    // // Second round ----------------------------
    // const int N2 = 3;
    // pthread_t threads2[N2];

    // barrier_set_N(&barrier, N2);

    // for (int i = 0; i < N2; i++) {
    //     pthread_create(&threads2[i], NULL, work, NULL);
    //     sleep(1);
    // }

    // for (int i = 0; i < N2; i++) {
    //     pthread_join(threads2[i], NULL);
    // }

    // printf("All the works have been done. Round 2\n");

    barrier_destroy(&barrier);

    return 0;
}