#include "bit_buffer.h"
#include <iostream>


//
// BitBuffer
//
BitBuffer::BitBuffer( std::string name )
{
    fileName          = name;
    buffer            = NULL;
    Reset();
}

BitBuffer::~BitBuffer()
{
    fileName.clear();
    Reset();
}

void BitBuffer::Reset()
{
    headerSize        = 0;  // Once header is specified, this is header's size.
    numOfBits         = 0;
    currentByteIdx    = 0;
    currentByte       = 0;
    bitsToGo          = 0;
    if( buffer )
    {
      delete[] buffer;
      buffer  = NULL;
    }
}


//
// InputBitBuffer
//
bool InputBitBuffer::Start()
{
    FILE* filePtr = std::fopen( fileName.c_str(), "r" );
    if( filePtr == NULL )
    {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return false;
    }
    std::fseek( filePtr, 0, SEEK_END );
    Int64 size = std::ftell( filePtr );
    std::rewind( filePtr );

    assert( buffer == NULL );
    try
    {
        buffer = new unsigned char[ size ];
    }
    catch (std::bad_alloc& ba)
    {
      std::cerr << "bad_alloc caught when reading file: " << ba.what() << std::endl;
      return false;
    }

    std::fread( buffer, sizeof(unsigned char), size, filePtr );
    std::fclose( filePtr );
    
    return true;
}

bool InputBitBuffer::End()
{
    Reset();
    return true;
}


bool InputBitBuffer::GetBit( Int32* bitValue )
{
    if( bitsToGo == 0 )
    {
      currentByte = buffer[ currentByteIdx++ ];
      bitsToGo = 8;
    }

    *bitValue = currentByte & 0x01;
    currentByte >>= 1;
    bitsToGo--;

    return true;
}


//
// OutputBitBuffer
//
void OutputBitBuffer::SetNumberOfBits( Int64 num )
{
    numOfBits = num;
}

bool OutputBitBuffer::Start()
{
  Int64 totalSize = headerSize + numOfBits / 8;
  if( numOfBits % 8 > 0 )
      totalSize++;
  try
  {
      buffer = new unsigned char[ totalSize ];
  }
  catch (std::bad_alloc& ba)
  {
      std::cerr << "bad_alloc caught when allocating output buffer: " << ba.what() << std::endl;
      return false;
  }

  return true;
}


bool OutputBitBuffer::End()
{
  Int64 totalSize = headerSize + numOfBits / 8;
  if( numOfBits % 8 > 0 )
  {
      totalSize++;
      // do something to pad the last few bits
  }

  FILE* f = std::fopen( fileName.c_str(), "w" );
  if( f == NULL )
  {
      std::cerr << "Failed to open file: " << fileName << std::endl;
      return false;
  }
  std::fwrite( buffer, sizeof(unsigned char), totalSize, f );
  std::fclose( f ); 

  Reset();

  return true;
}
