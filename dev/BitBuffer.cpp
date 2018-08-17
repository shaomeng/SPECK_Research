#include "BitBuffer.h"
#include <iostream>
#include <cassert>
#include <cstring>


void BitStreamHeader::PrintHeader() const
{
    std::cout << "Bitstream header info:" << std::endl;
    printf("  Data volume dimension          :  %hu x %hu x %hu\n", numCols, numRows, numFrames );
    printf("  Wavelet transform levels in XY :  %hhu\n", numLevelsXY );
    printf("  Wavelet transform levels in Z  :  %hhu\n", numLevelsZ  );
    printf("  Max coefficient bits           :  %hhu\n", maxCoefficientBits );
    printf("  Original data mean             :  %f\n", dataMean );
}

void BitStreamHeader::CopyToBuffer( unsigned char* buf ) const
{
    std::memcpy( buf,      &numLevelsXY,        1  );
    std::memcpy( buf + 1,  &numLevelsZ,         1  );
    std::memcpy( buf + 2,  &maxCoefficientBits, 1  );
    std::memcpy( buf + 3,  &numCols,            2  );
    std::memcpy( buf + 5,  &numRows,            2  );
    std::memcpy( buf + 7,  &numFrames,          2  );
    std::memcpy( buf + 9,  &dataMean,           8  );
} 

void BitStreamHeader::CopyFromBuffer( const unsigned char* buf )
{
    std::memcpy( &numLevelsXY,        buf,      1  );
    std::memcpy( &numLevelsZ,         buf + 1,  1  );
    std::memcpy( &maxCoefficientBits, buf + 2,  1  );
    std::memcpy( &numCols,            buf + 3,  2  );
    std::memcpy( &numRows,            buf + 5,  2  );
    std::memcpy( &numFrames,          buf + 7,  2  );
    std::memcpy( &dataMean,           buf + 9,  8  );
} 


//
// Constructor: BitBuffer
//
BitBuffer::BitBuffer( const std::string& name )
{
    // Make sure the data type sizes are as expected.
    if( (sizeof(UInt8) != 1) || (sizeof(UInt16) != 2) || (sizeof(Float64) != 8) )
    {
        std::cerr << "One or more data types have unexpected sizes: "
                  << "UInt8, UInt16, and Float64." << std::endl;
    }

    fileName          = name;
    buffer            = nullptr;
    Reset();
}

BitBuffer::~BitBuffer()
{
    fileName.clear();
    Reset();
}

void BitBuffer::Reset()
{
    headerSize        = header.GetSize();
    numOfBits         = 0;
    currentByte       = 0;
    bitsToGo          = 0;
    currentByteIdx    = headerSize;
    if( buffer )
    {
        delete[] buffer;
        buffer  = nullptr;
    }
    header.numLevelsXY = 0;
    header.numLevelsZ  = 0;
    header.maxCoefficientBits = 0;
    header.numCols     = 0;
    header.numRows     = 0;
    header.numFrames   = 0;
    header.dataMean    = 0.0;
}

void BitBuffer::PrintBitstream() const
{
    unsigned char   printByte;
    Int64           printByteIdx  = headerSize;
    Int32           printBitsToGo = 0;

    for( Int64 i = 0; i < numOfBits; i++ )
    {
        if( printBitsToGo == 0 )
        {
            printByte = buffer[ printByteIdx++ ];
            printBitsToGo = 8;
            std::cout << std::endl;
        }

        unsigned char printVal = printByte & 0x01;
        printByte >>= 1;
        printBitsToGo--;
        if( printVal == '\0' )
            printVal = '0';
        else 
            printVal = '1';
        std::cout << printVal << ", ";
    }
    std::cout << std::endl;
}


//
// InputBitBuffer
//
InputBitBuffer::InputBitBuffer( const std::string& name ) : BitBuffer( name )
{ }

bool InputBitBuffer::Start()
{
    FILE* filePtr = std::fopen( fileName.c_str(), "r" );
    if( filePtr == nullptr )
    {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return false;
    }
    std::fseek( filePtr, 0, SEEK_END );
    Int64 size = std::ftell( filePtr );
    std::rewind( filePtr );

    assert( buffer == nullptr );
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

    numOfBits  = (size - headerSize) * 8;
    header.CopyFromBuffer( buffer );
    
    return true;
}

bool InputBitBuffer::End()
{
    Reset();
    return true;
}


bool InputBitBuffer::GetBit( unsigned char* bitValue )
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
OutputBitBuffer::OutputBitBuffer( const std::string& name ) : BitBuffer( name )
{ }

void OutputBitBuffer::SetNumberOfBits( Int64 num )
{
    assert( num > 0 );
    numOfBits = num;
}

bool OutputBitBuffer::Start()
{
    bitsToGo    = 8;

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
    while( bitsToGo != 8 )  // didn't finish the last byte
        PutBit( 0 );

    Int64 totalSize = headerSize + numOfBits / 8;
    if( numOfBits % 8 > 0 )
        totalSize++;

    header.CopyToBuffer( buffer );

    FILE* f = std::fopen( fileName.c_str(), "w" );
    if( f == nullptr )
    {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return false;
    }
    std::fwrite( buffer, sizeof(unsigned char), totalSize, f );
    std::fclose( f ); 

    Reset();

    return true;
}

bool OutputBitBuffer::PutBit( unsigned char bitValue )
{
    currentByte >>= 1;
    if (bitValue)
        currentByte |= 0x80;
    bitsToGo--;

    if( bitsToGo == 0 )
    {
        buffer[ currentByteIdx++ ] = currentByte;
        currentByte                = 0;
        bitsToGo = 8;
    }

    return true;
}


int main()
{
    std::string filename = "test_buffer.bitstream";
    Int32       numbits  = 10;

    // write a bitstream to disk
    OutputBitBuffer  outbuf( filename );
    outbuf.header.numLevelsXY = 4;
    outbuf.header.numLevelsZ  = 3;
    outbuf.header.maxCoefficientBits = 11;
    outbuf.header.numCols     = 17;
    outbuf.header.numRows     = 12;
    outbuf.header.numFrames   = 53;
    outbuf.header.dataMean    = 0.53;
    outbuf.SetNumberOfBits( numbits );
    outbuf.Start();
    unsigned char a;
    for( Int32 i = 0; i < numbits; i++ )
    {
        std::cin >> a;
        if( a == '0' )
            a = '\0';
        outbuf.PutBit( a );
    }
    outbuf.End();

    // reads the bitstream from disk
    InputBitBuffer inbuf( filename );
    inbuf.Start();
    std::cout << "test input buffer: " << std::endl;
    inbuf.header.PrintHeader();
    inbuf.PrintBitstream();
    inbuf.End();
}
