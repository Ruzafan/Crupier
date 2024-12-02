#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

char *color_blue = "\033[01;34m";
char *color_end = "\033[00m";

void
error (char *m)
{
  write (2, m, strlen (m));
  write (2, "\n", 1);
  exit (0);
}

void * player_play(int pid, char * s, int * st)
{
  sprintf (s, "%s[%d] pid=%d created%s\n", color_blue, getpid (), pid,
          color_end);
  if (write (1, s, strlen (s)) < 0)
    error ("write");

  pid = wait (&st);
  if (pid == -1)
    error ("wait");

  sprintf (s, "%s[%d] pid=%d ended%s\n", color_blue, getpid (), pid,
          color_end);
  if (write (1, s, strlen (s)) < 0)
    error ("write");
}


int
main (int arc, char *arv[])
{
  int i, n, st, pid;
  char s[100];
  char *args[] = { "player1", NULL };

  if (arc != 2)
    error ("Error: wrong arguments.");

  n = atoi (arv[1]);
  pthread_t *threads = malloc(n * sizeof(pthread_t)); 
  if (threads == NULL) 
    error("malloc");

  sprintf (s,
           "\n**********Start of game (%d players, pid croupier=%d)***********\n",
           n, getpid ());
  if (write (1, s, strlen (s)) < 0)
    error ("write");

  for (i = 0; i < n; i++)
    {
      switch (pid = fork ())
        {
        case -1:
          error ("fork");

        case 0:
          execv (args[0], args);
          error ("exec");

        default:
        if (pthread_create(&threads[i], NULL, player_play(pid,s, &st), NULL) != 0)
            error("pthread_create");
        
         
        }
    }
  
  free(threads);

  sprintf (s, "\n**********End of game: all players have ended***********\n");
  if (write (1, s, strlen (s)) < 0)
    error ("write");

  exit (0);
}
