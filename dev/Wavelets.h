#include <cstdio>
#include <cstdlib>
#include <string>

// auto-vectorization tutorials:
// https://software.intel.com/sites/default/files/8c/a9/CompilerAutovectorizationGuide.pdf

namespace SPECK{

#define ALPHA     -1.58615986717275
#define BETA      -0.05297864003258
#define GAMMA      0.88293362717904
#define DELTA      0.44350482244527
#define EPSILON    1.14960430535816

/* the following is true on most intel machines */
using Float64 = double;     
using Float32 = float;      
using Int64   = long;
using Int32   = int;
using Int16   = short;

class Wavelets
{
public:

template <typename T>
void print_3D_data(T *data_3D, Int64 nFrame, Int64 nRow, Int64 nCol)
    //  
    // 3D Wavelet transform 
    //
    template< typename T >
    void ForwardTransform3D( T*           signal, 
                             Int64        xyDim, 
                             Int64        zDim, 
                             Int64        xyLevel, 
                             Int64        zLevel );

    //  
    // 3D Inverse Wavelet transform 
    //
    template< typename T >
    void InverseTransform3D( T*           signal, 
                             Int64        xyDim, 
                             Int64        zDim, 
                             Int64        xyLevel, 
                             Int64        zLevel );
};
};
