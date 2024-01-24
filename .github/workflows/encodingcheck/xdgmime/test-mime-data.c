/* test-mime-data.c: tests for the mime implementation
 *
 * More info can be found at http://www.freedesktop.org/standards/
 * 
 * Copyright (C) 2005  Red Hat, Inc.
 * Copyright (C) 2005  Matthias Clasen <mclasen@redhat.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later or AFL-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <libgen.h>

#include "xdgmime.h"

static int error = 0;
static int total = 0;
static int failed = 0;
static int xfailed = 0;
static int xmatch = 0;

static int verbose = 0;

static void
check_mime_type (const char *mt,
		 const char *mt_expected,
                 int         xfail,
		 const char *test,
		 const char *filename)
{
  total++;

  if (strcasecmp (mt, mt_expected) != 0)
    {
      failed++;
      
      if (xfail)
	{
	  xfailed++;
	  
	  if (verbose > 1)
	  printf ("%s, '%s' test: expected %s, got %s (expected failure)\n", 
		  filename, test, mt_expected, mt);
	}
      else
	{
	  if (verbose > 0)
	    fprintf (stderr, "%s, '%s' test: expected %s, got %s\n",
		     filename, test, mt_expected, mt);
	}

    }
  else
    {
      if (xfail)
	{
	  xmatch++;

	  if (verbose > 0)
	  printf ("%s, '%s' test: got %s (unexpected match)\n", 
		  filename, test, mt);
	}
    }

}

static void 
test_by_name (const char *filename,
	      const char *mt_expected,
              int         xfail)
{
  const char *mt;

  mt = xdg_mime_get_mime_type_from_file_name (filename);

  check_mime_type (mt, mt_expected, xfail, "name", filename);
}

static void 
test_by_data (const char *dir,
	      const char *filename,
	      const char *mt_expected,
              int         xfail)
{
  FILE  *file;
  const char *mt;
  int max_extent;
  char *data;
  int bytes_read;
  int result_prio;
  char path[1024];

  snprintf (path, 1024, "%s/%s", dir, filename);

  file = fopen (path, "r");

  if (file == NULL)
    {
      fprintf (stderr, "Could not open %s\n", path);
      error++;

      return;
    }

  max_extent = xdg_mime_get_max_buffer_extents ();
  data = malloc (max_extent);

  if (data == NULL)
    {
      fprintf (stderr, "Failed to allocate memory for file %s\n", filename);
      error++;

      fclose (file);
      return;
    }

  bytes_read = fread (data, 1, max_extent, file);
  
  if (ferror (file))
    {
      fprintf (stderr, "Error reading file %s\n", path);
      error++;

      free (data);
      fclose (file);
      
     return;
    }

  mt = xdg_mime_get_mime_type_for_data (data, bytes_read, &result_prio);
  
  free (data);
  fclose (file);
  
  check_mime_type (mt, mt_expected, xfail, "data", filename);
}

static void 
test_by_file (const char *dir,
	      const char *filename,
	      const char *mt_expected,
              int         xfail)
{
  const char *mt;
  char path[1024];

  snprintf (path, 1024, "%s/%s", dir, filename);

  mt = xdg_mime_get_mime_type_for_file (path, NULL);
  
  check_mime_type (mt, mt_expected, xfail, "file", filename);
}

static void
test_single_file (const char *dir,
		  const char *filename,
                  const char *mimetype,
                  int         xfail_name,
                  int         xfail_data,
                  int         xfail_file)
{
  test_by_name (filename, mimetype, xfail_name);
  test_by_data (dir, filename, mimetype, xfail_data);
  test_by_file (dir, filename, mimetype, xfail_file);
}

static void
read_from_file (const char *filename)
{
  FILE *file;
  char line[255];
  int lineno = 0;
  char *testfile, *mimetype, *flags;
  char *rest, *space, end;
  char *dir, *tmp;
  int xfail_name, xfail_data, xfail_file;
  
  file = fopen (filename, "r");
  tmp = strdup (filename);
  dir = strdup (dirname (tmp));
  free (tmp);

  if (file == NULL)
    {
      fprintf (stderr, "Could not open %s\n", filename);
      exit (1);
    }

  while (fgets (line, 255, file) != NULL)
    {
      lineno++;

      rest = line;
      while (*rest == ' ') rest++;
      
      if (*rest == '#' || *rest == '\n')
	continue;

      space = strchr (rest, ' ');
      if (!space)
	{
	  fprintf (stderr, "Malformed line in %s:%d\n\t%s\n", filename, lineno, line);
	  failed++;
	  continue;
	}

      *space = '\0';
      testfile = rest;

      rest = space + 1;
      while (*rest == ' ') rest++;
      space = rest;
      while (*space != ' ' && *space != '\n') space++;
      end = *space;

      *space = '\0';
      mimetype = rest;
      
      xfail_name = 0;
      xfail_data = 0;
      xfail_file = 0;

      if (end != '\n')
	{
	  rest = space + 1;
	  while (*rest == ' ') rest++;
	  space = rest;
	  while (*space != ' ' && *space != '\n') space++;
	  end = *space;
	  
	  *space = '\0';
          flags = rest;

          switch (strlen (flags))
           {
             default:
               printf ("%s.%d: Extra flags are ignored\n", filename, lineno);
               __attribute__((fallthrough));
             case 3:
               if (flags[2] == 'x')
                 xfail_file = 1;
               __attribute__((fallthrough));
             case 2:
               if (flags[1] == 'x')
                 xfail_data = 1;
               __attribute__((fallthrough));
             case 1: 
               if (flags[0] == 'x')
                 xfail_name = 1;
               break;
             case 0: ;
               /* Should not happen */
          }
        }

      test_single_file (dir, testfile, mimetype, 
                        xfail_name, xfail_data, xfail_file);
    }

  fclose (file);

  free (dir);
}

static void
usage (void)
{
  printf ("usage: test-mime-data <FILE>\n\n");
  printf ("Tests the mime system.\n");
  printf ("Testcases are specified in the following format:\n\n");
  printf ("# comment\n");
  printf ("<filename1> <mimetype> [<flags>]\n");
  printf ("<filename2> <mimetype> [<flags>]\n");
  printf ("...\n\n");
  printf ("Where an 'x' in the 1st, 2nd or 3rd position in <flags>\n");
  printf ("indicates an expected failure when determining the\n");
  printf ("mimetype by name, data or file.\n");

  exit (1);
}

int 
main (int argc, char *argv[])
{
  int i;

  if (argc < 2)
    usage ();
  
  for (i = 1; i < argc; i++)
    {
      if (strcmp (argv[i], "-v") == 0)
	verbose++;
      else
	read_from_file (argv[i]);
    }
  
  printf ("%d errors, %d comparisons, %d failed",
          error, total, failed);
  if (xfailed > 0)
    printf (" (%d expected)", xfailed);
  if (xmatch > 0)
    printf (", %d unexpected successes", xmatch);
  printf ("\n");

  if (error > 0 || xmatch > 0 || failed != xfailed)
    return 1;
  return 0;
}
