#include "Wavelets.h"

#include <string>

using namespace SPECK;


//
// Defines the header of a bit stream
//   Reference: http://qccpack.sourceforge.net/Documentation/QccSPECK3DDecode.3.html
//
struct BitStreamHeader
{
    UInt8    numLevelsZ;  // Number of levels of wavelet decomposition in Z direction
    UInt8    numLevelsXY; // Number of levels of wavelet decomposition in XY direction
    UInt16   numCols;     // X-axis dimension
    UInt16   numRows;     // Y-axis dimension
    UInt16   numFrames;   // Z-axis dimension
    Float64  dataMean;    // the mean value of the original data volume
    UInt8    maxCoefficientBits; // Indicates the precision, in number of bits, 
                                 // of the wavelet coefficient with the largest magnitude
};  // End of struct BitStreamHeader


class BitBuffer
{
public:
    BitBuffer( const std::string& name );       // Constructor, specify file name
   ~BitBuffer();                                // Destructor
    void Reset();
    void PrintSelf() const;                     // Caution: the last byte might not be correct.

    //
    // Each Start() call needs to be followed by an End(),
    //   before the next Start() call could function correctly.
    //
    virtual bool Start()    = 0;
    virtual bool End()      = 0;

    // 
    // Programmers could directly access the header and set values.
    //
    BitStreamHeader    header;

protected:
    std::string        fileName;
    Int64              headerSize;         // header size in byte
    Int64              numOfBits;          // total number of bits
    Int64              currentByteIdx;     // index of the current byte
    unsigned char*     buffer;
    unsigned char      currentByte;        // stores current byte;
    Int32              bitsToGo;           // how many bits untill finish currentByte

};  // End of class BitBuffer


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

};  // End of class InputBitBuffer


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

};  // End of class OutputBitBuffer
