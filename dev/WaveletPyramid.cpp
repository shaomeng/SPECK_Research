#include "WaveletPyramid.h"
#include <cmath>
#include <cstring>







// ----------------------------------------------------------------------------
// Class Set 
// ----------------------------------------------------------------------------
bool Set::IsSignificant( Int32 n )
{
    T   threshold = std::pow( 2.0, double(n) );
    T*  buf = new T[ dimX ];
    for( Int32 z = startZ; z < startZ + dimZ; z++ )
        for( Int32 y = startY; y < startY + dimY; y++ )
        {
            std::memcpy( buf, 
        }

    delete[] buf;
    return false;
}
