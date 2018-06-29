#include <cstdio>
#include <cstdlib>
#include <string>

namespace wavelet{

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

void ForwardTransform3D( Float64*     signal, 
                         Int64        xyDim, 
                         Int64        zDim, 
                         Int64        xyLevel, 
                         Int64        zLevel );

void InverseTransform3D( Float64*     signal, 
                         Int64        xyDim, 
                         Int64        zDim, 
                         Int64        xyLevel, 
                         Int64        zLevel );

// auto-vectorization tutorials:
// https://software.intel.com/sites/default/files/8c/a9/CompilerAutovectorizationGuide.pdf
};
