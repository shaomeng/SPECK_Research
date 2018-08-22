#ifndef WAVELETPYRAMID_H
#define WAVELETPYRAMID_H

#include "Wavelets.h"
#include <vector>

using namespace SPECK;


// ----------------------------------------------------------------------------
// WaveletPyramid class provides APIs to organize wavelet coefficients
//   into pyramid of blocks.
// ----------------------------------------------------------------------------
template <typename T>
class WaveletPyramid
{
public:
    const Int32   numCols;        // X axis dimension 
    const Int32   numRows;        // Y axis dimension 
    const Int32   numFrames;      // Z axis dimension 
    const Int32   numLevelsXY;    // XY plane wavelet decomposition levels
    const Int32   numLevelsZ;     // Z direction wavelet decomposition levels

    // Constructor
    WaveletPyramid( Int32 nc, Int32 nr, Int32 nf, Int32 nlxy, Int32 nlz ); 

    // Destructor
    ~WaveletPyramid();

    // WaveletPyramid takes over the ownership of this buffer.
    // Whoever owned buf previously should NOT touch this buffer again.
    // Upon successful, returns true. Otherwise, returns false, and the 
    //   old buffer content stays the same.
    bool HandOverBuffer( T* buf, Int64 numVals );

    // WaveletPyramid makes a copy of this buffer
    // Upon successful, returns true. Otherwise, returns false, and the 
    //   old buffer content may be corrupt.
    bool CopyOverBuffer( const float*  buf, Int64 numVals );
    bool CopyOverBuffer( const double* buf, Int64 numVals );

    const   T*    GetCoeffBuffer() const;

protected:
    T*            coeffBuffer;

};


// ----------------------------------------------------------------------------
// Set class defines a subblock (set) of a WaveletPyramid
// ----------------------------------------------------------------------------
template <typename T>
class Set
{
public:
    Int32   startX, dimX;
    Int32   startY, dimY;
    Int32   startZ, dimZ;

    // Default Constructor
    Set();
    
    // Custom Constructor
    Set( const WaveletPyramid<T>*   p,   
               Int32  sx,           Int32 dx, 
               Int32  sy,           Int32 dy,
               Int32  sz,           Int32 dz );

    bool    IsSignificant( Int32 n ) const;     // Is this set significant wrt n

    void    Activate( const WaveletPyramid<T>*   p,
                            Int32  sx,           Int32 dx, 
                            Int32  sy,           Int32 dy,
                            Int32  sz,           Int32 dz );
    bool    IsActive() const;
    void    Deactivate();
    bool    CoverSameBlock( const Set<T>& )   const;
    const   WaveletPyramid<T>*   GetPyramid() const;

    // Partition this Set into up to eight subsets
    std::vector<Set<T> >    Partition() const;

protected:
    const   WaveletPyramid<T>*   pyramid;       // The wavelet pyramid that this Set belongs to
    bool    active;                             // If true, this Set contains valid info.
                                                //   Otherwise, it could be activated and reused.
};

#endif
