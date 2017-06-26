#include "bit_buffer.h"
#include <iostream>


BitBuffer::BitBuffer( std::string name )
{
  fileName          = name;
  numOfFloats       = 0;
  numOfBits         = 0;
  buffer            = NULL;
}


BitBuffer::~BitBuffer()
{
  fileName.clear();
  Reset();
}


void BitBuffer::Reset()
{
  numOfFloats       = 0;
  numOfBits         = 0;
  if( buffer )
  {
    delete[] buffer;
    buffer  = NULL;
  }
}


void BitBuffer::SetNumOfFloats( Int64 num )
{
  numOfFloats = num;
}


bool InputBitBuffer::Start()
{
  FILE* filePtr = fopen( fileName.c_str(), "r" );
  if( filePtr == NULL )
  {
    std::cerr << "Failed to open file: " << fileName << std::endl;
    return false;
  }
  fseek( filePtr, 0, SEEK_END );
  Int64 size = ftell( filePtr );
  rewind( filePtr );
  try
  {
    buffer = new unsigned char[ size ];
  }
  catch (std::bad_alloc& ba)
  {
    std::cerr << "bad_alloc caught when reading file: " << ba.what() << std::endl;
    return false;
  }
  fread( buffer, sizeof(unsigned char), size, filePtr );
  fclose( filePtr );
  
  return true;
}


bool InputBitBuffer::End()
{
  Reset();
  return true;
}


bool OutputBitBuffer::Start()
{
  Int64 totalSize = numOfFloats * 8;
  totalSize      += numOfBits / 8;
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
  Int64 totalSize = numOfFloats * 8;
  totalSize      += numOfBits / 8;
  if( numOfBits % 8 > 0 )
  {
    totalSize++;
    // do something to pad the last few bits
  }

  FILE* f = fopen( fileName.c_str(), "w" );
  if( f == NULL )
  {
    std::cerr << "Failed to open file: " << fileName << std::endl;
    return false;
  }
  fwrite( buffer, sizeof(unsigned char), totalSize, f );
  fclose( f ); 

  Reset();

  return true;
}
