/**
  Reid Harry
  Computer Project #3
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
using namespace std;

int DecodePermissions(unsigned short s, char *out)
{
  if (s & 1)
    out[8] = 'x';
  if (s & 2)
    out[7] = 'w';
  if (s & 4)
    out[6] = 'r';
  if (s & 8)
    out[5] = 'x';
  if (s & 16)
    out[4] = 'w';
  if (s & 32)
    out[3] = 'r';
  if (s & 64)
    out[2] = 'x';
  if (s & 128)
    out[1] = 'w';
  if (s & 256)
    out[0] = 'r';
  return 1;
}

int PrintStats(const char *path, bool l)
{
  // Get stats about file
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
  {
    return -1;
  }

  // Short output
  // absolute pathname of the file
  printf("Path: %s\n", path);
  // permissions for the file
  char out[10] = "---------";
  DecodePermissions(statbuf.st_mode, out);
  printf("Permissions: %s\n", out);
  // size of the file (in bytes)
  printf("Size: %ld\n", statbuf.st_size);

  if (l)
  {
    // Long output
    // Numeric ID of the file's owner
    printf("User ID: %u\n", statbuf.st_uid);
    // Numeric ID of the file's group
    printf("Group ID: %u\n", statbuf.st_gid);
    // Time of last read from file
    printf("Last Read: %s", ctime(&statbuf.st_atime));
    // Time of last write from file
    printf("Last Write: %s", ctime(&statbuf.st_mtime));
    // Time of last status change
    printf("Last Status Change: %s", ctime(&statbuf.st_ctime));
  }

  printf("\n");

  return 1;
}

int main(int argc, char **argv)
{
  // True if user wants long output (false by default)
  bool l = false;

  // Get arguments
  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      if (argv[i][1] == 'L')
      {
        // -L argument
        l = true;
      }
      else if (argv[i][1] == 'S')
      {
        // -S argument
        l = false;
      }
      else
      {
        // Character other than 'a', 'b', or 't' follow a '-'
        printf("Invalid Argument: %s\n", argv[i]);
      }
    }
    else
    {
      // file argument
      string file = string(argv[i]);

      if (file.c_str()[0] != '/')
      {
        // Relative path

        // Try current directory
        char actualPath[256];
        realpath(file.c_str(), actualPath);
        PrintStats(actualPath, l);

        // Try DIRLIST
        string dirList = getenv("DIRLIST");
        if (dirList.c_str() == NULL)
        {
          // Ignore the DIRLIST if it does not exist
          return 1;
        }
        size_t start = 0;
        size_t end = dirList.find(':', start);
        while (end != string::npos)
        {
          // This if handles the special case of '.' being in the DIRLIST
          if (dirList.substr(start, end - start) == ".")
          {
            char actualPath[256];
            realpath(file.c_str(), actualPath);
            PrintStats(actualPath, l);
          }
          else
          {
            PrintStats((dirList.substr(start, end - start) + "/" + file).c_str(), l);
          }
          start = end + 1;
          end = dirList.find(':', start);
        }
        // This if handles the special case of '.' being in the DIRLIST
        if (dirList.substr(start, end - start) == ".")
          {
            char actualPath[256];
            realpath(file.c_str(), actualPath);
            PrintStats(actualPath, l);
          }
          else
          {
            PrintStats((dirList.substr(start, end - start) + "/" + file).c_str(), l);
          }
      }
      else
      {
        // Absolute path
        PrintStats(file.c_str(), l);
      }
    }
  }
}