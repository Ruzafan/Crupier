#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd[2], pid;
    char c;

    pid = fork();
    pipe(fd);
    
    if (pid == 0) {
        
        write(fd[1], "*", 1);
    } else {

        read(fd[0], &c, 1);
        write(1, &c, 1);
        write(1, "\n", 1);
    }

}
