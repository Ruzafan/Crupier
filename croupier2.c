#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char *color_blue = "\033[01;34m";
char *color_end = "\033[00m";

void
error (char *m)
{
    write (2, m, strlen (m));
    write (2, "\n", 1);
    exit (0);
}

int
main (int arc, char *arv[])
{
    int i, n, st, pid;
    int winner_pid, winner_score = -1;
    char s[100];
    char *args[] = { "player1", NULL };

    if (arc != 2)
        error ("Error: wrong arguments.");

    n = atoi (arv[1]);

    sprintf (s,
             "\n**********Start of game (%d players, pid croupier=%d)***********\n",
             n, getpid ());
    if (write (1, s, strlen (s)) < 0)
        error ("write");

    for (i = 0; i < n; i++) {
        switch (pid = fork()) {
        case -1:
            error ("fork");

        case 0:
            execv (args[0], args);
            error ("exec");

        default:
            sprintf (s, "%s[%d] pid=%d created%s\n", color_blue, getpid (), pid, color_end);
            if (write (1, s, strlen (s)) < 0)
                error ("write");
        }
    }

    // Wait for all players to finish
    for (i = 0; i < n; i++) {
        pid = wait(&st);
        if (pid == -1)
            error("wait");
        int player_score = WEXITSTATUS(st);
        sprintf (s, "%s[%d] pid=%d ended%s\n", color_blue, getpid (), pid, color_end);
        if (write (1, s, strlen (s)) < 0)
            error ("write");
        if (player_score > winner_score) {
            winner_pid = pid;
            winner_score = player_score;
        } else if (player_score == winner_score && pid < winner_pid) {
            // In case of a tie, the player with the lower PID wins
            winner_pid = pid;
        }
    }

    sprintf (s, "\n**********End of game: all players have ended***********\n");
    if (write (1, s, strlen (s)) < 0)
        error ("write");

    exit (0);
}