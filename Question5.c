#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void create_children(int num_children) {
    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            pause(); // Wait for signal
            exit(0);
        }
    }
}

int main() {
    pid_t pids[5];
    for (int i = 0; i < 5; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            // Child process
            create_children(i + 1);
            pause(); // Wait for signal
            exit(0);
        }
    }

    // Parent process
    sleep(1); // Wait for a second

    if (fork() == 0) {
        // Child process to execute pstree
        char pid_str[10];
        sprintf(pid_str, "%d", getppid());
        execlp("pstree", "pstree", "-c", "-p", pid_str, (char *)NULL);
        exit(0);
    }

    // Wait for all child processes
    for (int i = 0; i < 5; i++) {
        waitpid(pids[i], NULL, 0);
    }

    pause(); // Wait for signal
    return 0;
}
