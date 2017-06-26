#include <string>
#include <cstdio>

#include "wavelet.h"
using namespace wavelet;

class BitBuffer
{
public:
  BitBuffer( std::string name);       // constructor, specify file name
 ~BitBuffer();                        // Destructor
  void SetNumOfFloats( Int64 num );   // This is for the header
  void Reset();                       //

  virtual bool Start()    = 0;
  virtual bool End()      = 0;

protected:
  std::string        fileName;
  Int64              numOfFloats;        // total number of 64-bit floats as header
  Int64              numOfBits;          // total number of bits
  unsigned char*     buffer;
};


//
// Reads a bitstream from disk
//
class InputBitBuffer : BitBuffer
{
public:
  bool Start();
  bool End();

  bool GetBit( Int32 &bitValue );     // zero means 0, non-zero means 1
  bool GetHeader( Float64* header );  // reads the header: numOfFloats 64-bit floats
};


//
// Outputs a bitstream to disk
//
class OutputBitBuffer : BitBuffer
{
public:
  bool Start();
  bool End();
  void SetNumberOfBits( Int64 num );

  bool PutBit( Int32 &bitValue );     // zero means 0, non-zero means 1
  bool PutHeader( Float64* header );  // puts the header: numOfFloats 64-bit floats

private:
  Int64              byte_cnt;
  Int64              bit_cnt;
  Int32              bits_to_go;    // bits left to fill the current byte
};
