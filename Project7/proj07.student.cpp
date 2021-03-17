/*  Reid Harry
 *  Computer Project #7
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
using namespace std;

typedef unsigned short int uint16;

class RegisterUnit
{
  private:

    uint16 reg[16];

  public:

    // Construct and initialize
    //
    RegisterUnit()
    {
      for (unsigned i=0; i<16; i++)
        reg[i] = 0x00;
    }

    // Return reference to element I
    //
    uint16& operator[]( unsigned I )
    {
      return reg[I&0xf];
    }

    // Return constant reference to element I
    //
    const uint16& operator[]( unsigned I ) const
    {
      return reg[I&0xf];
    }

    // Display RegisterUnit
    //
    void display( std::ostream& out ) const
    {
      std::ios old( nullptr );
      old.copyfmt( out );

      out << std::setfill( '0' ) << std::hex;

      for (unsigned i=0; i<4; i++)
      {
        unsigned n = i;
        out << "R" << std::setw(1) << n << ": "
            << std::setw(4) << reg[n] << "  ";
        n = n+4;
        out << "R" << std::setw(1) << n << ": "
            << std::setw(4) << reg[n] << "  ";
        n = n+4;
        out << "R" << std::setw(1) << n << ": "
            << std::setw(4) << reg[n] << "  ";
        n = n+4;
        out << "R" << std::setw(1) << n << ": "
            << std::setw(4) << reg[n] << std::endl;
      }

      out.copyfmt( old );
    }
};



class DataCacheEntry
{
private:
    bool V = false;
    bool M = false;
    uint16 tag = 0x00;
public:
    void SetV(bool v) { V = v; }
    bool GetV() { return V; }

    void SetM(bool m) { M = m; }
    bool GetM() { return M; }

    void SetTag(uint16 t) { tag = t; }
    bool GetTag() { return tag; }
    
     
};

int main(int argc, char **argv)
{
    bool debugMode = false;
    bool ramMode = false;
    string fileName = "";

    // Get command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if ((string)argv[i] == "-input")
            {
                // Input argument
                fileName = argv[i+1];
                i++;
            }
            else if ((string)argv[i] == "-debug")
            {
                // Debug argument
                debugMode = true;
            }
            else if ((string)argv[i] == "-ram")
            {
                // Ram argument
                ramMode = true;
            }
            else
            {
                printf("Invalid argument! Will try to continue execution without it.\n");
            }
        }
    }

    // Open the file
    FILE* file = fopen(fileName.c_str(), "r");
    if (file == NULL)
    {
        // file not found
        printf("ERROR: the input argument was not found.\n");
        exit(-1);
    }

    // Traverse the file
    char operation[256];
    uint16 regNum = -1;
    uint16 address = -1;
    while (fscanf(file, "%255s %x %x", operation, &regNum, &address) == 3)
    {
        
    }

    cout << "Debug mode: " << debugMode << endl;
    cout << "Ram mode: " << ramMode << endl;
    if (fileName != "")
    {
        cout << "File name: " << fileName << endl;
    }
    
}
