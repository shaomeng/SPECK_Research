#include "WaveletPyramid.h"
#include <cmath>
#include <cstring>


// ----------------------------------------------------------------------------
// Class WaveletPyramid
// ----------------------------------------------------------------------------

// Constructor
template<typename T>
WaveletPyramid<T>::WaveletPyramid( Int32 nc, Int32 nr, Int32 nf, Int32 nlxy, Int32 nlz )
    : numCols( nc ), numRows( nr ), numFrames( nf ), numLevelsXY( nlxy ), numLevelsZ( nlz )
{
    coeffBuffer = nullptr;
}

template<typename T>
const T* WaveletPyramid::GetCoeffBuffer() const
{
    return coeffBuffer;
}

bool WaveletPyramid::HandOverBuffer( T* buf, Int64 numVals )
{
    if( numVals != (Int64)numCols * numRows * numFrames )
        return false;

    if( coeffBuffer )
        delete[] coeffBuffer;
    coeffBuffer = buf;

    return true;
}

bool WaveletPyramid::HandOverBuffer( const float* buf, Int64 numVals )
{
    if( numVals != (Int64)numCols * numRows * numFrames )
        return false;
    
    if( coeffBuffer )
        delete[] coeffBuffer;

    coeffBuffer = new (std::nothrow) T[ numVals ];
    if( !coeffBuffer )
        return false;

    for( Int64 i = 0; i < numVals; i++ )
        coeffBuffer[i] = (T)buf[i];

    return true;
}

bool WaveletPyramid::HandOverBuffer( const double* buf, Int64 numVals )
{
    if( numVals != (Int64)numCols * numRows * numFrames )
        return false;
    
    if( coeffBuffer )
        delete[] coeffBuffer;

    coeffBuffer = new (std::nothrow) T[ numVals ];
    if( !coeffBuffer )
        return false;

    for( Int64 i = 0; i < numVals; i++ )
        coeffBuffer[i] = (T)buf[i];

    return true;
}






// ----------------------------------------------------------------------------
// Class Set 
// ----------------------------------------------------------------------------

// Constructor
Set::Set( const WaveletPyramid* P, Int32 sx, Int32 dx, Int32 sy, Int32 dy, Int32 sz, Int32 dz) 
   : pyramid( p ), startX( sx ), dimX( dx ), startY( sy ), dimY( dy ), startZ( sz ), dimZ( dz )
{}

bool Set::IsSignificant( Int32 n )
{
    T   threshold   = std::pow( 2.0, double(n) );
    T*  buf         = new T[ dimX ];
    const T* coeffs = pyramid->GetCoeffBuffer();
    for( Int32 z = startZ; z < startZ + dimZ; z++ )
        for( Int32 y = startY; y < startY + dimY; y++ )
        {
            std::memcpy( buf, coeffs + z * pyramid.numCols * pyramid.Rows +
                                       y * pyramid.numCols + startX, sizeof(T) * dimX );
            for( Int32 x = 0; x < dimX; x++ )
                if( buf[x] > threshold )
                {
                    delete[] buf;
                    return true;
                }
        }

    delete[] buf;
    return false;
}


// Explicit Template Instantiation
class template WaveletPyramid<float>;
class template WaveletPyramid<double>;
class template Set<float>;
class template Set<double>;
