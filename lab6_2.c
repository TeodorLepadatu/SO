#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int row;
    int col;
    int p;             
    int **mat1;
    int **mat2;
    int **mat3;
} ThreadData;

void* getElement(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int sum = 0;
    for (int i = 0; i < data->p; i++) {
        sum += data->mat1[data->row][i] * data->mat2[i][data->col];
    }
    data->mat3[data->row][data->col] = sum;
    pthread_exit(0);
}

int main() {
    int m, p, k;  // m x p matrix * p x k matrix = m x k result matrix
    scanf("%d %d %d", &m, &p, &k);

    int **mat1 = (int **)malloc(m * sizeof(int *));
    int **mat2 = (int **)malloc(p * sizeof(int *));
    int **mat3 = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) mat1[i] = (int *)malloc(p * sizeof(int));
    for (int i = 0; i < p; i++) mat2[i] = (int *)malloc(k * sizeof(int));
    for (int i = 0; i < m; i++) mat3[i] = (int *)malloc(k * sizeof(int));

    for (int i = 0; i < m; i++)
        for (int j = 0; j < p; j++)
            scanf("%d", &mat1[i][j]);

    for (int i = 0; i < p; i++)
        for (int j = 0; j < k; j++)
            scanf("%d", &mat2[i][j]);

    pthread_t threads[m][k];
    ThreadData thread_data[m][k];

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            thread_data[i][j].row = i;
            thread_data[i][j].col = j;
            thread_data[i][j].p = p;
            thread_data[i][j].mat1 = mat1;
            thread_data[i][j].mat2 = mat2;
            thread_data[i][j].mat3 = mat3;

            if (pthread_create(&threads[i][j], NULL, getElement, &thread_data[i][j]) != 0) {
                perror("eroare ba");
                return 1;
            }
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            printf("%d ", mat3[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < m; i++) free(mat1[i]);
    for (int i = 0; i < p; i++) free(mat2[i]);
    for (int i = 0; i < m; i++) free(mat3[i]);
    free(mat1);
    free(mat2);
    free(mat3);

    return 0;
}

