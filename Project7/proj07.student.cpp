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
    uint16 data[8];
public:
    DataCacheEntry()
    {
        V = false;
        M = false;
        tag = 0x00;
    }

    uint16& operator[]( unsigned I )
    {
      return data[I];
    }

    const uint16& operator[]( unsigned I ) const
    {
      return data[I];
    }

    void SetV(bool v) { V = v; }
    bool GetV() { return V; }

    void SetM(bool m) { M = m; }
    bool GetM() { return M; }

    void SetTag(uint16 t) { tag = t; }
    bool GetTag() { return tag; }
};

class DataCache
{
private:
    DataCacheEntry cache[8];
public:
    DataCache()
    {
        for (unsigned int i = 0; i < 8; i++)
        {
            cache[i] = DataCacheEntry();
        }
    }

    DataCacheEntry& operator[]( unsigned I )
    {
      return cache[I];
    }

    const DataCacheEntry& operator[]( unsigned I ) const
    {
      return cache[I];
    }

    void display()
    {
        printf("     V M Tag  0  1  2  3  4  5  6  7\n");
        printf("------------------------------------\n");

        for (unsigned int i = 0; i < 8; i++)
        {
            printf("[%u]: %u %u %03x %02x %02x %02x %02x %02x %02x %02x %02x\n",
                i, cache[i].GetV(), cache[i].GetM(), cache[i].GetTag(),
                cache[i][0], cache[i][1], cache[i][2], cache[i][3], cache[i][4],
                cache[i][5], cache[i][6], cache[i][7]);
        }
    }
};

class RAM
{
private:
    uint16 content[65536];
public:
    RAM()
    {
        for (unsigned int i = 0; i < 65536; i++)
        {
            content[i] = 0x00;
        }
    }

    uint16& operator[]( unsigned I )
    {
      return content[I];
    }

    const uint16& operator[]( unsigned I ) const
    {
      return content[I];
    }

    void display()
    {
        for (int i = 0; i < 128; i+=16)
        {            
            printf("%04x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
                i, content[i], content[i+1], content[i+2], content[i+3],
                content[i+4], content[i+5], content[i+6], content[i+7],
                content[i+8], content[i+9], content[i+10], content[i+11],
                content[i+12], content[i+13], content[i+14], content[i+15], content[i+16]);
        }
    }
};

int main(int argc, char **argv)
{
    RegisterUnit registers;
    DataCache cache;
    RAM ram;

    bool debugMode = false;
    bool ramMode = false;
    string ramName = "";
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
                ramName = argv[i+1];
                i++;
            }
            else
            {
                printf("Invalid argument! Will try to continue execution without it.\n");
            }
        }
    }

    // Open ram file if ram flag set
    if (ramMode)
    {
        FILE* ramFile = fopen(ramName.c_str(), "r");
        if (ramFile == NULL)
        {
            // File not found
            printf("ERROR: the ram input argument was not found.\n");
            exit(-1);
        }

        unsigned int ramAddress = -1;
        unsigned int ramLine[16];
        while (fscanf(ramFile, "%04x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
            &ramAddress, &ramLine[0], &ramLine[1], &ramLine[2], &ramLine[3],
            &ramLine[4], &ramLine[5], &ramLine[6], &ramLine[7],
            &ramLine[8], &ramLine[9], &ramLine[10], &ramLine[11],
            &ramLine[12], &ramLine[13], &ramLine[14], &ramLine[15]) == 17)
        {
            for (int i = 0; i < 16; i++)
            {
                ram[ramAddress + i] = ramLine[i];
            }
        }
    }

    // Display debug info if necessary
    if (debugMode)
    {
        printf("CONTENTS OF REGISTERS, CACHE, AND RAM BEFORE INPUT INSTRUCTION:\n");

        registers.display(cout);

        printf("\n");
        cache.display();

        printf("\n");
        ram.display();
        printf("\n");
    }

    // Open the file
    FILE* file = fopen(fileName.c_str(), "r");
    if (file == NULL)
    {
        // file not found
        printf("ERROR: the file input argument was not found.\n");
        exit(-1);
    }

    // Traverse the file
    char operation[256];
    unsigned int regNum = -1;
    unsigned int address = -1;
    while (fscanf(file, "%255s %x %x", operation, &regNum, &address) == 3)
    {
        int offset = (address & 0x00000007);
        int line = (address & 0x00000038) >> 3;
        int tag = (address & 0x0000FFC0) >> 6;

        printf("%s %01x %04x %03x %01x %01x\n", 
            operation, regNum, address,
            tag, line, offset);
        if (debugMode)
        {
            cache.display();
        }
    }

    printf("\n");
    printf("CONTENTS OF REGISTERS, CACHE, AND RAM AFTER INPUT INSTRUCTION:\n");
    registers.display(cout);

    printf("\n");
    cache.display();

    printf("\n");
    ram.display();
    
}
