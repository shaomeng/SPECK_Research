#include "WaveletPyramid.h"
#include <cmath>
#include <cstring>
#include <new>
#include <cstdio>
#include <forward_list>


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

// Default Constructor
template <typename T>
Set<T>::Set()
{
    pyramid = nullptr;
    startX  = 0;        dimX = 0;
    startY  = 0;        dimY = 0;  
    startZ  = 0;        dimZ = 0;
}

// Custom Constructor
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
    return (pyramid != nullptr);
}

template <typename T>
void Set<T>::Deactivate()
{
    pyramid = nullptr;
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
}

template <typename T>
const WaveletPyramid<T>* Set<T>::GetPyramid() const
{
    return pyramid;
}

template <typename T>
std::vector<Set<T> > Set<T>::Partition() const
{
    //    y2      z2
    //     |     /
    //     |    /
    //    y1   z1
    //     |  /
    //     | /
    //    xyz0----x1----x2

    Int32  x[3] = { startX, startX + dimX / 2, startX + dimX };
    Int32  y[3] = { startY, startY + dimY / 2, startY + dimY };
    Int32  z[3] = { startZ, startZ + dimZ / 2, startZ + dimZ }; 

    std::vector<Set<T> > subsets( 8 );
    subsets[0].Activate( pyramid, x[0], x[1] - x[0],
                                  y[0], y[1] - y[0],
                                  z[0], z[1] - z[0] );
    subsets[1].Activate( pyramid, x[1], x[2] - x[1],
                                  y[0], y[1] - y[0],
                                  z[0], z[1] - z[0] );
    subsets[2].Activate( pyramid, x[0], x[1] - x[0],
                                  y[1], y[2] - y[1],
                                  z[0], z[1] - z[0] );
    subsets[3].Activate( pyramid, x[1], x[2] - x[1],
                                  y[1], y[2] - y[1],
                                  z[0], z[1] - z[0] );

    subsets[4].Activate( pyramid, x[0], x[1] - x[0],
                                  y[0], y[1] - y[0],
                                  z[1], z[2] - z[1] );
    subsets[5].Activate( pyramid, x[1], x[2] - x[1],
                                  y[0], y[1] - y[0],
                                  z[1], z[2] - z[1] );
    subsets[6].Activate( pyramid, x[0], x[1] - x[0],
                                  y[1], y[2] - y[1],
                                  z[1], z[2] - z[1] );
    subsets[7].Activate( pyramid, x[1], x[2] - x[1],
                                  y[1], y[2] - y[1],
                                  z[1], z[2] - z[1] );

    for( Int32 i = 7; i >= 0; i-- )
    {
        if( (subsets[i].dimX == 0) || (subsets[i].dimY == 0) || (subsets[i].dimZ == 0) )
            subsets[i].Deactivate();
        else
        {
            for( Int32 j = 0; j < i; j++ )
            {
                if( subsets[i].CoverSameBlock( subsets[j] ) )
                {
                    subsets[i].Deactivate();
                    break;
                }
            }
        }
    }

    return subsets; 
}

template <typename T>
bool Set<T>::CoverSameBlock( const Set<T>& that ) const
{
    return ( (startX == that.startX) && (dimX == that.dimX) &&
             (startY == that.startY) && (dimY == that.dimY) &&
             (startZ == that.startZ) && (dimZ == that.dimZ)    );
}

template <typename T>
bool Set<T>::IsSingleVertex() const
{
    return ( (dimX == 1) && (dimY == 1) && (dimZ == 1) );
}

template <typename T>
void Set<T>::PrintInfo() const
{
    printf("  (%d -- %d, %d -- %d, %d -- %d)\n", startX, startX + dimX, 
                                                 startY, startY + dimY, 
                                                 startZ, startZ + dimZ );
}


// Explicit Template Instantiation
template class WaveletPyramid<Float32>;
template class WaveletPyramid<Float64>;
template class Set<Float32>;
template class Set<Float64>;


int main()
{
    Int32 dimX = 2, dimY = 2, dimZ = 1;

    WaveletPyramid<Float32> pyramid( dimX, dimY, dimZ, 3, 3 );

    Set<Float32> set1( &pyramid, 0, dimX, 0, dimY, 0, dimZ );

    std::forward_list<Set<Float32> > allSets = { set1 };
    
    std::vector<Set<Float32> > sets = set1.Partition();   
    allSets.insert_after( allSets.cbefore_begin(), sets.cbegin(), sets.cend() );
    allSets.remove_if( [](const Set<Float32>& s){ return !s.IsActive(); } );
    for( const Set<Float32>& it : allSets )
        it.PrintInfo();
}
