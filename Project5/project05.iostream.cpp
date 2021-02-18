
/******************************************************************************
  Demonstrate iostream class library functions
******************************************************************************/

#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
   int A = 99, B = 99, C = 99;

   const double
     X = 19.95,
     Y = 2.2250738585072014E-308,    // Smallest possible double
     Z = 1.7976931348623157E+308;    // Largest possible double

   cout << endl << "Please enter three integer values: ";
   cin >> A >> B >> C;

   cout << endl;
   cout << "A: " << A << endl;
   cout << "B: " << B << endl;
   cout << "C: " << C << endl << endl;

   cout << "const double" << endl;
   cout << "  X = 19.95," << endl;
   cout << "  Y = 2.2250738585072014E-308," << endl;
   cout << "  Z = 1.7976931348623157E+308;" << endl << endl;

   cout << "X: " << X << endl;
   cout << "Y: " << Y << endl;
   cout << "Z: " << Z << endl << endl;

   cout << resetiosflags( ios::fixed ) << setiosflags( ios::scientific );

   cout << "X: " << X << endl;
   cout << "Y: " << Y << endl;
   cout << "Z: " << Z << endl << endl;

   cout << setprecision( 8 );

   cout << "X: " << X << endl;
   cout << "Y: " << Y << endl;
   cout << "Z: " << Z << endl << endl;

   cout << setprecision( 30 );

   cout << "X: " << X << endl;
   cout << "Y: " << Y << endl;
   cout << "Z: " << Z << endl << endl;

   cout << resetiosflags( ios::scientific ) << setiosflags( ios::fixed );
   cout << setprecision( 5 );

   cout << "X: " << setw( 10 ) << X << endl;
   cout << "Y: " << setw( 10 ) << Y << endl << endl;

   cout << "[" << setw( 12 ) << "CSE 325" << "]" << endl << endl;
   cout << "[" << setw( 12 ) << A << "]" << endl << endl;

   cout << setiosflags( ios::left );

   cout << "[" << setw( 12 ) << "CSE 325" << "]" << endl << endl;
   cout << "[" << setw( 12 ) << A << "]" << endl << endl;

   cout << setiosflags( ios::right ) << setfill( '*' ) << setprecision( 2 );

   cout << "Amount:  $" << setw( 8 ) << X << endl << endl;
}

