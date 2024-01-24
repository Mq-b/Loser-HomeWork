/* print-mime-data.c: debug tests for the mime implementation
 *
 * More info can be found at http://www.freedesktop.org/standards/
 * 
 * Copyright (C) 2005  Red Hat, Inc.
 * Copyright (C) 2005  Matthias Clasen <mclasen@redhat.com>
 * Copyright (C) 2012  Bastien Nocera <hadess@hadess.net>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later or AFL-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>

#include "xdgmime.h"

static void
usage (void)
{
  printf ("usage: print-mime-data <DIR>\n\n");
  printf ("Prints the mime-type of every file in <DIR>, detected in various ways.\n");

  exit (1);
}

static void
test_by_name (const char *filename)
{
  const char *mt;

  mt = xdg_mime_get_mime_type_from_file_name (filename);

  printf ("\tname: %s\n", mt);
}

static void
test_by_data (const char *filename)
{
  FILE  *file;
  const char *mt;
  int max_extent;
  char *data;
  int bytes_read;
  int result_prio;

  file = fopen (filename, "r");

  if (file == NULL)
    {
      printf ("Could not open %s\n", filename);
      return;
    }

  max_extent = xdg_mime_get_max_buffer_extents ();
  data = malloc (max_extent);

  if (data == NULL)
    {
      printf ("Failed to allocate memory for file %s\n", filename);
      fclose (file);
      return;
    }

  bytes_read = fread (data, 1, max_extent, file);
  
  if (ferror (file))
    {
      printf ("Error reading file %s\n", filename);

      free (data);
      fclose (file);
      
     return;
    }

  mt = xdg_mime_get_mime_type_for_data (data, bytes_read, &result_prio);
  
  free (data);
  fclose (file);

  printf ("\tdata: %s\n", mt);
}

static void
test_by_file (const char *filename)
{
  const char *mt;

  mt = xdg_mime_get_mime_type_for_file (filename, NULL);

  printf ("\tfile: %s\n", mt);
}

static int
is_regular (const char *filename)
{
  struct stat s;

  if (stat (filename, &s) == 0)
    if (S_ISREG (s.st_mode))
      return 1;

  return 0;
}

static void
process_file (const char *dir, const char *filename)
{
  char path[1024];

  snprintf (path, 1024, "%s/%s", dir, filename);

  if (!is_regular (path))
    return;

  printf ("%s:\n", filename);

  test_by_name (filename);
  test_by_data (path);
  test_by_file (path);

  printf ("\n");
}

static void
read_from_dir (const char *path)
{
  DIR *dir;
  struct dirent *entry;

  dir = opendir (path);
  if (!dir) {
    printf ("Could not open dir '%s'\n", path);
    return;
  }

  entry = readdir (dir);
  while (entry != NULL) {
    if (strcmp (entry->d_name, ".") == 0 ||
        strcmp (entry->d_name, "..") == 0)
      goto next;
    process_file (path, entry->d_name);

next:
    entry = readdir (dir);
  }

  closedir (dir);
}

int
main (int argc, char *argv[])
{
  int i;

  if (argc < 2)
    usage ();
  
  for (i = 1; i < argc; i++)
    {
      read_from_dir (argv[i]);
    }

  return 0;
}
