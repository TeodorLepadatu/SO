#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

int collatz(int *shm_ptr, int x, int i) {
    shm_ptr[i] = x; // tine numaru in shm
    if (x == 1) {
        return i + 1; // returneaza cat a scris
    }
    x = (x % 2) ? 3 * x + 1 : x / 2;
    return collatz(shm_ptr, x, i + 1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number1> <number2> ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Starting parent %d\n", getpid());

    // shared memory setup
    char shm_name[] = "collatz";
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd < 0) {
        perror("shm_open");
        return errno;
    }

    // allocate shared memory 
    size_t shm_size = getpagesize() * (argc - 1); // o pozitie per numar
    if (ftruncate(shm_fd, shm_size) == -1) {
        perror("ftruncate");
        shm_unlink(shm_name);
        return errno;
    }

    pid_t pids[argc - 1];
    for (int i = 0; i < argc - 1; i++) {
        if ((pids[i] = fork()) < 0) {
            perror("fork");
            return errno;
        } else if (pids[i] == 0) {
            // child process: map its part of shared memory
            int *shm_ptr = mmap(0, getpagesize(), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, i * getpagesize());
            if (shm_ptr == MAP_FAILED) {
                perror("mmap");
                return errno;
            }

            int n = atoi(argv[i + 1]);
            int used_len = collatz(shm_ptr, n, 0);

            printf("Done Parent %d Me %d\n", getppid(), getpid());

            munmap(shm_ptr, getpagesize());
            exit(0);
        }
    }

    // wait
    for (int i = 0; i < argc - 1; i++) {
        wait(NULL);
    }

    // parent prints 
    for (int i = 0; i < argc - 1; i++) {
        int *shm_ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, shm_fd, i * getpagesize());
        if (shm_ptr == MAP_FAILED) {
            perror("mmap");
            return errno;
        }

        printf("%s: ", argv[i + 1]);
        for (int j = 0; shm_ptr[j] != 1; j++) {
            printf("%d ", shm_ptr[j]);
        }
        printf("1.\n");

        munmap(shm_ptr, getpagesize());
    }

    // Cleanup
    shm_unlink(shm_name);

    printf("Done Parent %d Me %d\n", getpid(), getppid());
    return 0;
}

