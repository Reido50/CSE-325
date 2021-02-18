/**
  Reid Harry
  Computer Project #3
*/

#include<stdio.h>
#include<iostream>
#include<string>
#include<string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main(int argc, char** argv)
{
 int bufferLength = 64;
 int flags = O_CREAT;

 bool a = false;
 bool t = false;

 string fileA;
 string fileB;
 
 // Get arguments
 for (int i = 1; i < argc; i++)
 {
  if (argv[i][0] == '-')
  {
   if (argv[i][1] == 'b')
   {
     // -b argument

     // Determine if the arugment afterward is a number
     bool isNumber = true;
     for (int j = 0; j < (int)strlen(argv[i+1]-1); j++)
     {
       if (!isdigit(argv[i][j]))
       {
         isNumber = false;
         break;
       }
     } 

     // If it is, set new bufferLength
     if (isNumber)
     {
       bufferLength = stoi(argv[i+1]);
       i++;
     }
     else
     {
       // User didn't supply a number after the -b
       printf("The -b argument must be followed by a number \n");
     }
   }
   else if (argv[i][1] == 'a')
   {
     // -a argument
      
     if (!t)
     {
      // or O_APPEND to the flags 
      flags = flags | O_APPEND;
      a = true;
     }
     else
     {
      // User can't use both -a and -t
      printf("Cannot use both -a and -t\n");
      exit(0);
     }
   }
   else if (argv[i][1] == 't')
   {
    // -t arguemnt
    
    if (!a)
    {
     // or O_TRUNC to the flags
     flags = flags | O_TRUNC;
     t = true;
    }
    else
    {
     // User can't use both -a and -t
     printf("Cannot use both -a and -t\n");
     exit(0);
    }
   }
   else
   {
    // Character other than 'a', 'b', or 't' follow a '-'
    printf("Invalid Argument: %s\n", argv[i]);
   } 
  }
  else if (fileA.length() == 0)
  {
   // File A is found (input file)
   fileA = string(argv[i]);
  }
  else if (fileB.length() == 0)
  {
   // File B is found (output file)
   fileB = string(argv[i]);
  }
  else
  {
   // Both fileA and fileB are filled, so this argument is invalid
   printf("Invalid Number Of Inputs/Outputs \n");
  }
 }

 if (fileA.length() == 0 || fileB.length() == 0)
 {
  // Missing some arguments
  printf("Missing argument(s)\n");
  exit(0);
 }

 // Open the files
 int Afd = open(fileA.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
 int Bfd = open(fileB.c_str(), O_WRONLY | flags, S_IRUSR | S_IWUSR);

 /** Help with initializing buffer came from:
  * https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
 */
 int numReadBytes = bufferLength;
 
 // Read/Write loop
 while (numReadBytes > 0)
 {
  char* buffer = (char*) calloc(bufferLength, sizeof(char));
  numReadBytes = read(Afd, buffer, bufferLength);
  if (numReadBytes == 0)
  {
   break;
  }
  write(Bfd, buffer, bufferLength);
 }

 // Close the files
 close(Afd);
 close(Bfd);
}

