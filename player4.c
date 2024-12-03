#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TRUE	1
#define FALSE	0

char *color_red = "\033[01;31m";
char *color_yellow = "\033[01;33m";
char *color_green = "\033[01;32m";
char *color_end = "\033[00m";

void
error (char *m)
{
  write (2, m, strlen (m));
  write (2, "\n", 1);
  exit (0);
}

void
initializations()
{
  /* Initialize random numbers */
  srand (getpid ());
}


/* Encoding cards: 0:king, 1:ace, 2:2, ..., 7:7, 8:jack, 9:horse */
/* To not handle decimals, I save the numeric value multiplied by 2 */

#define NUM_CARDS 10
struct
{
  char *nom;
  int valor;
} cards[NUM_CARDS] =
{
  {
  "king", 1},
  {
  "ace", 2},
  {
  "2", 4},
  {
  "3", 6},
  {
  "4", 8},
  {
  "5", 10},
  {
  "6", 12},
  {
  "7", 14},
  {
  "jack", 1},
  {
  "horse", 1}
};

/* Return a card (integer between 0 and NUM_CARDS -1) */
int
get_card ()
{
  return (rand () % NUM_CARDS);
}

int
main ()
{
  int no_end = TRUE;
  int current = 0, previous = 0;
  int card;
  char s[100];

  initializations ();

  while (no_end)
    {
      card = get_card ();

      previous = current;
      current += cards[card].valor;

      sprintf
        (s,
         "[%d] valor previous=%2.1f, card received=%s, valor current=%2.1f\n",
         getpid (), previous / 2.0, cards[card].nom, current / 2.0);
      if (write (1, s, strlen (s)) < 0)
        error ("write");

      if (current > 15)
        {                       /* 15 = 7.5 * 2 */
          sprintf (s, "%s[%d] I've passed.\n%s", color_red, getpid (),
                   color_end);
          no_end = FALSE;
        }
      else if (current == 15)
        {
          sprintf (s, "%s[%d] I stand with 7.5\n%s", color_green, getpid (),
                   color_end);
          no_end = FALSE;
        }
      else
        {
          if ((rand () % 3) == 0)
            {
              sprintf (s, "%s[%d] I decide to stand with %2.1f\n%s",
                       color_yellow, getpid (), current / 2.0, color_end);
              no_end = FALSE;
            }
          else
            {
              sprintf (s, "[%d] I decide to continue.\n", getpid ());
            }
        }
      if (write (1, s, strlen (s)) < 0)
        error ("write");

      /* To introduce some randomness in the players' runtime */
      sleep (rand () % 2);
    }

  FILE *fp = fopen("results.txt", "a");
  if (fp == NULL) {
      perror("fopen");
      exit(1);
  }
  fprintf(fp, "%d %d\n", getpid(), current);
  fclose(fp);
  exit(0);
}
