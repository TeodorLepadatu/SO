#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void collatz(int n) {
    while (n != 1) {
        printf("%d\n", n);
        if (n % 2 == 0) {
            n /= 2;  
        } else {
            n = 3 * n + 1;  
        }
    }
    printf("%d\n", n);  
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return errno;
    } else if (pid == 0) {
        collatz(n);
        exit(0);
    } else {
        int status;
        wait(NULL);
        printf("d", n);
    }

    return 0;
}

