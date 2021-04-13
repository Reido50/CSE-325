/*  Reid Harry
 *  Computer Project #9
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <string.h>
using namespace std;

typedef unsigned short int uint16;

class PageTableEntry
{
private:
    bool V = false;
    bool W = false;
    bool P = false;
    bool R = false;
    bool M = false;
    uint16 frame;
public:
    PageTableEntry()
    {
        V = false;
        W = false;
        P = false;
        R = false;
        M = false;
        frame = 0x00;
    }

    void SetV(bool v) { V = v; }
    bool GetV() { return V; }

    void SetW(bool w) { W = w; }
    bool GetW() { return W; }

    void SetP(bool p) { P = p; }
    bool GetP() { return P; }

    void SetR(bool r) { R = r; }
    bool GetR() { return R; }

    void SetM(bool m) { M = m; }
    bool GetM() { return M; }

    void SetFrame(uint16 f) { frame = f; }
    uint16 GetFrame() { return frame; }
};

class PageTable
{
private:
    PageTableEntry pages[16];
public:
    PageTable() {}

    PageTableEntry& operator[]( unsigned I )
    {
      return pages[I];
    }

    const PageTableEntry& operator[]( unsigned I ) const
    {
      return pages[I];
    }

    void display()
    {
        printf("     V W P R M FN\n");
        printf("-----------------\n");

        for (unsigned int i = 0; i < 16; i++)
        {
            printf("[%01x]: %u %u %u %u %u %02x\n",
                i, pages[i].GetV(), pages[i].GetW(), pages[i].GetP(),
                pages[i].GetR(), pages[i].GetM(), pages[i].GetFrame());
        }
    }
};

int main(int argc, char **argv)
{
    PageTable pageTable;

    bool procMode = false;
    bool debugMode = false;
    string procName = "";
    string refsName = "";

    // Get command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if ((string)argv[i] == "-proc")
            {
                // Input argument
                procMode = true;
                procName = argv[i+1];
                i++;
            }
            else if ((string)argv[i] == "-debug")
            {
                // Debug argument
                debugMode = true;
            }
            else if ((string)argv[i] == "-refs")
            {
                // Ram argument
                refsName = argv[i+1];
                i++;
            }
            else
            {
                printf("Invalid argument! Will try to continue execution without it.\n");
            }
        }
    }

    // Open and traverse proc if needed
    if (procMode)
    {
        // Open the file
        FILE* file = fopen(procName.c_str(), "r");
        if (file == NULL)
        {
            // file not found
            printf("ERROR: the file input argument was not found.\n");
            exit(-1);
        }
        // Traverse the file
        int pageNum;
        int writePerm;
        while (fscanf(file, "%x %u", &pageNum, &writePerm) == 2)
        {
            pageTable[pageNum].SetW((bool)writePerm);
            pageTable[pageNum].SetV(true);
        }
    }

    // Debug pageTable display before sim
    if (debugMode)
    {
        printf("CONTENTS OF PAGE TABLE AT SIMULATION START:\n");
        pageTable.display();
        printf("\n");
    }

     // Open the file
    FILE* file = fopen(refsName.c_str(), "r");
    if (file == NULL)
    {
        // file not found
        printf("ERROR: the file input argument was not found.\n");
        exit(-1);
    }

    // Traverse the file
    char operation[256];
    unsigned int virtualAddress = -1;
    int readCount = 0;
    int writeCount = 0;
    while (fscanf(file, "%255s %x", operation, &virtualAddress) == 2)
    {
        int offset = (virtualAddress & 0x000003FF);
        int pageNum = (virtualAddress & 0x00003C00) >> 10;

        if ((string)operation == "RD")
        {
            readCount++;
        }
        else if ((string)operation == "WR")
        {
            writeCount++;
        }

        printf("%s %04x %01x %03x\n", 
            operation, virtualAddress,
            pageNum, offset);
        if (debugMode)
        {
            pageTable.display();
        }
    }

    // Display read/write totals after sim
    printf("\n");
    printf("Total number of read operations: %u\nTotal number of write operations: %u\n",
        readCount, writeCount);
    printf("\n");

    // Display pageTable after sim
    printf("CONTENTS OF PAGE TABLE AT SIMULATION END:\n");
    pageTable.display();
}
