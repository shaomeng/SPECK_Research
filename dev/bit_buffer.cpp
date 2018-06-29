#include "bit_buffer.h"
#include <iostream>
#include <cassert>

//
// Constructor: BitBuffer
//
BitBuffer::BitBuffer( const std::string& name )
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
    currentByte       = 0;
    bitsToGo          = 0;
    currentByteIdx    = headerSize;
    if( buffer )
    {
        delete[] buffer;
        buffer  = NULL;
    }
}

void BitBuffer::PrintSelf() const
{
    unsigned char   printByte;
    unsigned char   printByteIdx  = headerSize;
    Int16           printBitsToGo = 0;;

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
    Int32       numbits  = 9;

    OutputBitBuffer  outbuf( filename );
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

    InputBitBuffer inbuf( filename );
    inbuf.Start();
    std::cout << "test input buffer: " << std::endl;
    for( Int32 i = 0; i < numbits; i++ )
    {
        inbuf.GetBit( &a );
        if( a == '\0' )
            a = '0';
        else 
            a = '1';
        std::cout << a << ", " << std::endl;
    }
    inbuf.End();
}
