/*  Reid Harry
 *  Computer Project #10
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <string.h>
#include <queue>
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
    int timeStamp = -1;
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

    void SetTime(int t) { timeStamp = t; }
    int GetTime() { return timeStamp; }
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

    int FindReplacement()
    {
        // Find LRU of the valid entries
        int leastUsed = 0;
        for (unsigned int i = 0; i < 16; i++)
        {
            if (pages[i].GetV() && (pages[i].GetTime() < pages[leastUsed].GetTime()))
            {
                leastUsed = i;
            }
        }

        // Return LRU
        return leastUsed;
    }
};

int main(int argc, char **argv)
{
    PageTable pageTable;

    bool procMode = false;
    bool debugMode = false;
    string procName = "";
    string refsName = "";

    unsigned int frameNumStart = 0x20;
    unsigned int processFrames = 0;

    queue<unsigned int> availableFrames;

    int readCount = 0;
    int writeCount = 0;
    int pageFaultCount = 0;
    int writeBackCount = 0;

    int timeCount = 0;

    // Get command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if ((string)argv[i] == "-proc")
            {
                // Input argument
                procMode = true;
                if (i + 1 < argc)
                {
                    procName = argv[i+1];
                }
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
                if (i + 1 < argc)
                {
                    refsName = argv[i+1];
                }
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

        // Read first line of proc file and init availableFrames accordingly
        fscanf(file, "%d", &pageNum);
        processFrames = pageNum;
        for (unsigned int i = 0; i < processFrames; i++)
        {
            availableFrames.push(frameNumStart + i);
        }

        // Go through remainder of proc file
        while (fscanf(file, "%x %u", &pageNum, &writePerm) == 2)
        {
            /*
            if (!availableFrames.empty())
            {
                // Frame was found for the PTE
                pageTable[pageNum].SetFrame(availableFrames.front());
                availableFrames.pop();
            }
            else
            {
                // Frame was not found for PTE, so we must evict the LRU 
                int victim = pageTable.FindReplacement();
            
                // Determine if write back
                if (pageTable[victim].GetM())
                {
                    writeBackCount++;
                }
            }
            */
            // Init PTE
            pageTable[pageNum].SetW((bool)writePerm);
            pageTable[pageNum].SetV(true);
            pageTable[pageNum].SetTime(timeCount);
            timeCount++;
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
    unsigned int virtualAddress = 0;
    while (fscanf(file, "%255s %x", operation, &virtualAddress) == 2)
    {
        // Init flags
        char pageFaultFlag = ' ';
        char writeBackFlag = ' ';

        // Extract offset and page number
        int offset = (virtualAddress & 0x000003FF);
        int pageNum = (virtualAddress & 0x00003C00) >> 10;
        
        // Check for invalid page frame
        if (!pageTable[pageNum].GetV())
        {
            // V == 0
            printf("INVALID MEMORY REFERENCE AT PAGE NUMBER %01x\n", pageNum);
            continue;
        }
        else if ((string)operation == "WR" && !pageTable[pageNum].GetW())
        {
            // Attempt to write on a PTE with W == 0
            printf("PAGE NUMBER %01x IS NOT WRITABLE\n", pageNum);
            continue;
        }

        // Handle operation
        unsigned int physicalAddress = 0;
        // Check for invalid page
        if (!pageTable[pageNum].GetP())
        {
            // Page fault, so update count and flag
            pageFaultCount++;
            pageFaultFlag = 'F';

            // Check if eviction is needed
            if (availableFrames.empty())
            {
                // Find LRU victim
                int victim = pageTable.FindReplacement();
            
                // Determine if write back
                if (pageTable[victim].GetM())
                {
                    writeBackCount++;
                    writeBackFlag = 'F';
                }

                pageTable[victim].SetP(false);
                availableFrames.push(pageTable[victim].GetFrame());
            }
            
            // Set up PTE
            pageTable[pageNum].SetFrame(availableFrames.front());
            availableFrames.pop();
            pageTable[pageNum].SetP(true);
            pageTable[pageNum].SetR(false);
            pageTable[pageNum].SetM(false);
        }

        if ((string)operation == "RD")
        {
            readCount++;
            pageTable[pageNum].SetR(true);
            pageTable[pageNum].SetTime(timeCount);
            timeCount++;
        }
        else if ((string)operation == "WR")
        {
            writeCount++;
            pageTable[pageNum].SetR(true);
            pageTable[pageNum].SetM(true);
            pageTable[pageNum].SetTime(timeCount);
            timeCount++;
        }

        physicalAddress = (offset | (pageTable[pageNum].GetFrame() << 10));

        printf("%s %04x %01x %03x %c %c %04x\n", 
            operation, virtualAddress,
            pageNum, offset, pageFaultFlag, writeBackFlag, physicalAddress);

        if (debugMode)
        {
            pageTable.display();
        }
    }

    // Display read/write totals after sim
    printf("\n");
    printf("Total number of read operations: %u\nTotal number of write operations: %u\n",
        readCount, writeCount);
    printf("Total number of page faults: %u\nTotal number of write backs: %u\n",
        pageFaultCount, writeBackCount);
    printf("\n");

    // Display pageTable after sim
    printf("CONTENTS OF PAGE TABLE AT SIMULATION END:\n");
    pageTable.display();

    

}
