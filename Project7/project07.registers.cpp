
#include <iostream>
#include <iomanip>

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


int main()
{
  RegisterUnit A;

  std::cout << "Register Unit" << std::endl;
  A.display( std::cout );
  std::cout << std::endl;

  std::cout << "A[3]: " << A[3] << std::endl;
  std::cout << std::endl;

  A[3] = A[3] + 0xab;
  
  std::cout << "A[3]: " << A[3] << std::endl;
  std::cout << std::endl;

  std::cout << "Register Unit" << std::endl;
  A.display( std::cout );
  std::cout << std::endl;
}

