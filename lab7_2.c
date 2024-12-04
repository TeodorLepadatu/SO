#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NR_THREADS 5

pthread_mutex_t mtx;
sem_t sem;

int count = 0;

int barrier_point() {
    pthread_mutex_lock(&mtx);
    count++;
    if (count < NR_THREADS) {
        pthread_mutex_unlock(&mtx);
        sem_wait(&sem);  // blochez pana ajung toate
    } else {
        // da drumul la bariera
        for (int i = 0; i < NR_THREADS - 1; i++) {
            sem_post(&sem);
        }
        pthread_mutex_unlock(&mtx);
    }
    return 0;
}

void* tfun(void* v) {
    int* tid = (int*)v;	//id-ul threadului

    printf("%d reached the barrier\n", *tid);
    barrier_point();
    printf("%d passed the barrier\n", *tid);

    free(tid);
    return NULL;
}

int main() {
    int i;
    int* id;
    pthread_t thr[NR_THREADS];

    if (pthread_mutex_init(&mtx, NULL)) {
        perror(NULL);
        return errno;
    }

    if (sem_init(&sem, 0, 0)) {  
        perror(NULL);
        return errno;
    }

    for (i = 0; i < NR_THREADS; i++) {
        id = (int*)malloc(sizeof(int));
        *id = i;
        if (pthread_create(&thr[i], NULL, tfun, id)) {
            perror(NULL);
            return errno;
        }
    }

    for (i = 0; i < NR_THREADS; i++) {
        if (pthread_join(thr[i], NULL)) {
            perror(NULL);
            return errno;
        }
    }

    sem_destroy(&sem);
    pthread_mutex_destroy(&mtx);

    return 0;
}

