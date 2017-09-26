/*========================================================================
WSLRUN
main.c
Copyright (c)2017 Kevin Boone, GPL v3.0
========================================================================*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*========================================================================
  showUsage
========================================================================*/
void showUsage (FILE *f, const char *argv0)
  {
  fprintf (stderr, "Usage: %s {executable} [args...]\n", argv0); 
  }


/*========================================================================
  main 
========================================================================*/
int main (int argc, char **argv)
  {
  BOOL debug = FALSE;

  if (argc < 2)
    {
    showUsage (stderr, argv[0]);
    return EINVAL;
    }

  if (getenv ("WSLRUN_DEBUG"))
    debug = TRUE;

  // Length of new_argv is the same as original argv, because we don' t
  //  need argv[0], but we do need a terminating NULL
  char **new_argv = malloc (argc * sizeof (char *));
  new_argv[0] = argv[1];

  int i;
  for (i = 2; i < argc; i++)
    {
    BOOL munge = FALSE;
    char *p = canonicalize_file_name (argv[i]);
    if (debug)
      printf ("i=%d e=%d \"%s\" argv=%s canonical=%s\n" , i, errno, strerror(errno), argv[i], p);
    char *new_path;
 
    if (p)
      {
      if (strlen (p) < 5) break;
      if (strncmp (p, "/mnt/", 5) == 0 && p[6] == '/')
        {
        char drive_letter = p[5];
        new_path = malloc (strlen (p) + 3);
        strcpy (new_path + 2, p + 6);
        new_path[0] = drive_letter;
        new_path[1] = ':';
        int j, l = strlen (new_path);
        for (j = 0; j < l; j++)
          {
          if (new_path[j] == '/') new_path[j] = '\\';
          }
        munge = TRUE;
        }
      }

    if (munge)
      new_argv[i-1] = new_path; 
    else
      new_argv[i-1] = argv[i];
    }

  new_argv[i-1] = 0;

  if (debug)
    {
    int j = 0;
    char *s;
    do
      {
      s = new_argv[j];
      if (s)
        printf ("%d: %s\n", j, s);
      j++;
      } while (s); 
    }

  execvp (new_argv[0], new_argv); 
  }

