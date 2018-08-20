#include "Wavelets.h"

using namespace SPECK;


// ----------------------------------------------------------------------------
// WaveletPyramid class provides APIs to organize wavelet coefficients
//   into pyramid of blocks.
// ----------------------------------------------------------------------------
template <typename T>
class WaveletPyramid
{

private:
    T*      coeffBuffer;
    Int32   numCols;        // X axis dimension 
    Int32   numRows;        // Y axis dimension 
    Int32   numFrame;       // Z axis dimension 
    Int32   numLevelsXY;    // XY plane wavelet decomposition levels
    Int32   numLevelsZ;     // Z direction wavelet decomposition levels
};


// ----------------------------------------------------------------------------
// Set class defines a subblock (set) of a WaveletPyramid
// ----------------------------------------------------------------------------
template <typename T>
class Set
{
public:
    bool    IsSignificant( Int32 n );     // Is this set significant wrt n

private:
    const   WaveletPyramid*   pyramid;    // The wavelet pyramid that this Set belongs to
    Int32   startX, dimX;
    Int32   startY, dimY;
    Int32   startZ, dimZ;
};
