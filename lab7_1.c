#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
pthread_mutex_t mtx;

int decrease_count(int count) {
    pthread_mutex_lock(&mtx);
    if (available_resources < count) {
        pthread_mutex_unlock(&mtx);
        return -1;
    } else {
        available_resources -= count;
        printf("Got %d resources %d remaining\n", count, available_resources);
        pthread_mutex_unlock(&mtx);
    }
    return 0;
}

int increase_count(int count) {
    pthread_mutex_lock(&mtx);
    available_resources += count;
    printf("Released %d resources %d remaining\n", count, available_resources);
    pthread_mutex_unlock(&mtx);
    return 0;
}

void* sim(void* arg) {
    int count = *((int*)arg);
    if (decrease_count(count) == 0) {
        sleep(1);	//asta ca sa para ca face ceva :)
        increase_count(count);
    } else {
        printf("Not enough resources for %d resources request\n", count);
    }
    return NULL;
}

int main() {
    pthread_t threads[5];
    int resources_requested[5] = {2, 2, 1, 3, 2};

    pthread_mutex_init(&mtx, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, sim, &resources_requested[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mtx);
    return 0;
}

