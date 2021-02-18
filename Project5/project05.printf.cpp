
/******************************************************************************
  Demonstrate the use of "printf" and "scanf"
******************************************************************************/

#include <stdio.h>

int main()
{
  const char   A = '?';
  const short  B = 19;
  const int    C = 28;
  const float  D = 23.5;
  const double E = 16.25;
  const char   F[] = "CSE 325";

  int Count, X;
  double Y;
  char Z;

  printf( "A: %2c  %2d  %x\n\n", A, A, A );
  printf( "B: %2d  %2x  %04x\n\n", B, B, B );
  printf( "C: %2d  %2X  %08X\n\n", C, C, C );
  printf( "D: %f  %5.2f  %e  %6.4e\n\n", D, D, D, D );
  printf( "E: %f  %5.2f  %E  %6.4E\n\n", E, E, E, E );
  printf( "F: >%s<  >%10s<  >%-10s<\n\n", F, F, F );

  printf( "Please enter an integer, a real and a character: " );
  Count = scanf( "%d %lf %c", &X, &Y, &Z );

  if (Count > 0)
  {
    printf( "X:  %d  Y:  %g  Z:  %c\n\n", X, Y, Z );
  }

  return 0;
}

