#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char s[80];
    int n = 0;

    if (fork() == 0) n = 2 * n; else n = 2 * n + 1;
    if (fork() == 0) n = 2 * n; else n = 2 * n + 1;
    if (fork() == 0) n = 2 * n; else n = 2 * n + 1;

    sprintf(s, "%d:%d:%d\n", getpid(), getppid(), n);
    write(1, s, strlen(s));

    sleep(1);
    exit(0);
}
