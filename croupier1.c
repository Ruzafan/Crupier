#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

char *color_blue = "\033[01;34m";
char *color_end = "\033[00m";

void error(char *m)
{
  write(2, m, strlen(m));
  write(2, "\n", 1);
  exit(0);
}

void *run_player(void *arg)
{
  char *args[] = {"player1", NULL};
  int pid = getpid();
  char s[100];

  sprintf(s, "%s[%d] Player thread started%s\n", color_blue, pid, color_end);
  if (write(1, s, strlen(s)) < 0)
    error("write");

  execv(args[0], args);
  error("exec");
  return NULL; // Just to satisfy the return type
}

int main(int arc, char *arv[])
{
  if (arc != 2)
    error("Error: wrong arguments.");

  int n = atoi(arv[1]);
  char s[100];

  sprintf(s, "\n**********Start of game (%d players, pid croupier=%d)***********\n", n, getpid());
  if (write(1, s, strlen(s)) < 0)
    error("write");

  pthread_t *threads = malloc(n * sizeof(pthread_t));
  if (threads == NULL)
    error("malloc");

  for (int i = 0; i < n; i++)
  {
    if (pthread_create(&threads[i], NULL, run_player, NULL) != 0)
      error("pthread_create");

    sprintf(s, "%s[%d] Thread %d created%s\n", color_blue, getpid(), i, color_end);
    if (write(1, s, strlen(s)) < 0)
      error("write");
  }

  for (int i = 0; i < n; i++)
  {
    if (pthread_join(threads[i], NULL) != 0)
      error("pthread_join");
  }

  free(threads);

  sprintf(s, "\n**********End of game: all players have ended***********\n");
  if (write(1, s, strlen(s)) < 0)
    error("write");

  exit(0);
}
