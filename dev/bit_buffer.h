#include "wavelet.h"
using namespace wavelet;

class BitBuffer
{
public:
    BitBuffer( const std::string& name );       // Constructor, specify file name
   ~BitBuffer();                                // Destructor
    void Reset();
    void PrintSelf() const;

    // Each Start() call needs to be followed by an End(),
    //   before the next Start() call.
    virtual bool Start()    = 0;
    virtual bool End()      = 0;

protected:
    std::string        fileName;
    Int64              headerSize;         // header size in byte
    Int64              numOfBits;          // total number of bits
    Int64              currentByteIdx;     // index of the current byte
    unsigned char*     buffer;
    unsigned char      currentByte;        // stores current byte;
    Int32              bitsToGo;           // how many bits untill finish currentByte
};


//
// Reads a bitstream from disk
//
class InputBitBuffer : public BitBuffer
{
public:
    //InputBitBuffer( const std::string& name );
    bool Start();
    bool End();
    bool GetBit( Int32* bitValue );     // zero means 0, non-zero means 1
};


//
// Outputs a bitstream to disk
//
class OutputBitBuffer : public BitBuffer
{
public:
    OutputBitBuffer( const std::string& name );
    bool Start();
    bool End();
    void SetNumberOfBits( Int64 num );
    bool PutBit( Int32 bitValue );      // zero means 0, non-zero means 1
};
