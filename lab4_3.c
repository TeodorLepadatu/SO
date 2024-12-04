#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void collatz(int n) {
    printf("%d: ", n);
    while (n != 1) {
        printf("%d ", n);
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
    }
    printf("1.\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number1> <number2> ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Starting parent %d\n", getpid());

    for (int i = 1; i < argc; i++) {
        int n = atoi(argv[i]);
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Child process
            collatz(n);
            printf("Done Parent %d Me %d\n", getppid(), getpid());
            exit(0); // Exit child process
        }
        // Parent process continues to the next iteration
    }

    // Parent waits for all child processes to finish
    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }

    printf("Done Parent %d Me %d\n", getpid(), getppid());
    return 0;
}

