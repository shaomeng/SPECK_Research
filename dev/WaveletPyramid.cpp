#include "WaveletPyramid.h"
#include <cmath>
#include <cstring>
#include <new>


// ----------------------------------------------------------------------------
// Class WaveletPyramid
// ----------------------------------------------------------------------------

// Constructor
template <typename T>
WaveletPyramid<T>::WaveletPyramid( Int32 nc, Int32 nr, Int32 nf, Int32 nlxy, Int32 nlz )
    : numCols( nc ), numRows( nr ), numFrames( nf ), numLevelsXY( nlxy ), numLevelsZ( nlz )
{
    coeffBuffer = nullptr;
}

// Destructor
template <typename T>
WaveletPyramid<T>::~WaveletPyramid()
{
    delete[] coeffBuffer;
}

template <typename T>
const T* WaveletPyramid<T>::GetCoeffBuffer() const
{
    return coeffBuffer;
}

template <typename T>
bool WaveletPyramid<T>::HandOverBuffer( T* buf, Int64 numVals )
{
    if( numVals != (Int64)numCols * numRows * numFrames )
        return false;

    if( coeffBuffer )
        delete[] coeffBuffer;
    coeffBuffer = buf;

    return true;
}

template <typename T>
bool WaveletPyramid<T>::CopyOverBuffer( const Float32* buf, Int64 numVals )
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

template <typename T>
bool WaveletPyramid<T>::CopyOverBuffer( const Float64* buf, Int64 numVals )
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
template <typename T>
Set<T>::Set( const WaveletPyramid<T>*   p, 
                   Int32 sx,            Int32 dx, 
                   Int32 sy,            Int32 dy, 
                   Int32 sz,            Int32 dz ) 
{
    pyramid = p;
    startX  = sx;        dimX = dx;
    startY  = sy;        dimY = dy;  
    startZ  = sz;        dimZ = dz;
    active  = true;
}

// Copy Constructor
template <typename T>
Set<T>::Set( const Set& set2 )
{
    pyramid = set2.GetPyramid();
    startX  = set2.startX;      dimX = set2.dimX;
    startY  = set2.startY;      dimY = set2.dimY;
    startZ  = set2.startZ;      dimZ = set2.dimZ;
    active  = set2.IsActive();
}

template <typename T>
bool Set<T>::IsSignificant( Int32 n ) const
{
    T   threshold   = std::pow( 2.0, double(n) );
    T*  buf         = new T[ dimX ];
    const T* coeffs = pyramid->GetCoeffBuffer();
    for( Int32 z = startZ; z < startZ + dimZ; z++ )
        for( Int32 y = startY; y < startY + dimY; y++ )
        {
            std::memcpy( buf, coeffs + z * pyramid->numCols * pyramid->numRows +
                                       y * pyramid->numCols + startX, sizeof(T) * dimX );
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

template <typename T>
bool Set<T>::IsActive() const
{
    return active;
}

template <typename T>
void Set<T>::Deactivate()
{
    pyramid = nullptr;
    active  = false;
}

template <typename T>
void Set<T>::Activate( const WaveletPyramid<T>*     p,
                             Int32 sx,              Int32 dx, 
                             Int32 sy,              Int32 dy, 
                             Int32 sz,              Int32 dz ) 
{
    pyramid = p;
    startX  = sx;        dimX = dx;
    startY  = sy;        dimY = dy;  
    startZ  = sz;        dimZ = dz;
    active  = true;
}

template <typename T>
const WaveletPyramid<T>* Set<T>::GetPyramid() const
{
    return pyramid;
}


// Explicit Template Instantiation
template class WaveletPyramid<Float32>;
template class WaveletPyramid<Float64>;
template class Set<Float32>;
template class Set<Float64>;
