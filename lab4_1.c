#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        return errno;
    } else if (pid == 0) {
        char *argv[] = {"/bin/ls", NULL}; 
        execve("/bin/ls", argv, NULL);
        perror("execve failed");
        exit(errno);
    } else {
        printf("PID parinte = %d, PID copil = %d\n", getpid(), pid);
        wait(NULL);
        printf("s-a term copilul %d \n", pid);
    }
    
    return 0;
}

