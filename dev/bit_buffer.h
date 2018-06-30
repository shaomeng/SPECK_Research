#include "wavelet.h"
using namespace wavelet;

class BitBuffer
{
public:
    BitBuffer( const std::string& name );       // Constructor, specify file name
   ~BitBuffer();                                // Destructor
    void Reset();
    void PrintSelf() const;                     // Caution: the last byte might not be correct.

    //
    // Each Start() call needs to be followed by an End(),
    // before the next Start() call.
    //
    virtual bool Start()    = 0;
    virtual bool End()      = 0;

protected:
    std::string        fileName;
    Int64              headerSize;         // header size in byte
    Int64              numOfBits;          // total number of bits
    Int64              currentByteIdx;     // index of the current byte
    unsigned char*     buffer;
    unsigned char      currentByte;        // stores current byte;
    Int16              bitsToGo;           // how many bits untill finish currentByte
};


//
// Reads a bitstream from disk
//
class InputBitBuffer : public BitBuffer
{
public:
    InputBitBuffer( const std::string& name );
    bool Start();
    bool End();
    //
    // GetBit() should be called in between Start() and End().
    // It is the responsibility of the programmer to not get more bits 
    // than what this buffer has.
    //
    bool GetBit( unsigned char* bitValue );     // zero means 0, non-zero means 1
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
    //
    // PutBit() should be called in between Start() and End().
    // It is the responsibility of the programmer to not put more bits 
    // than what this buffer is instructed to hold by SetNumberOfBits().
    //
    bool PutBit( unsigned char bitValue );      // zero means 0, non-zero means 1
};
