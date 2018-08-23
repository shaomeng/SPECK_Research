// auto-vectorization tutorials:
// https://software.intel.com/sites/default/files/8c/a9/CompilerAutovectorizationGuide.pdf

#ifndef WAVELETS_H
#define WAVELETS_H

namespace SPECK{

#define ALPHA     -1.58615986717275
#define BETA      -0.05297864003258
#define GAMMA      0.88293362717904
#define DELTA      0.44350482244527
#define EPSILON    1.14960430535816

/* the following is true on most intel machines */
using Float64 = double;     
using Float32 = float;      
using Int64   = long int;
using Int32   = int;
using Int16   = short int;
using UInt32  = unsigned int;
using UInt16  = unsigned short;
using UInt8   = unsigned char;


class Wavelets
{
public:
    // 
    // Helper function
    //
    template <typename T>
    void print_3D_data(T *data_3D, Int64 nFrame, Int64 nRow, Int64 nCol);

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
    //
    // 1D Wavelet transform
    //
    template <typename T>
    void ForwardTransform1D( T* signal, Int64 length, Int64 nLevel );

    //
    // 1D Inverse Wavelet transform
    //
    template <typename T>
    void InverseTransform1D( T* signal, Int64 length, Int64 nLevel );

    //
    // 2D Wavelet transform
    //
    template <typename T>
    void ForwardTransform2D( T* signal, Int64 dim, Int64 nLevel );

    //
    // 2D Inverse Wavelet transform
    //
    template <typename T>
    void InverseTransform2D( T* signal, Int64 dim, Int64 nLevel );

protected:
    template <typename T>
    void QccWAVCDF97AnalysisSymmetricEvenEven( T* signal, Int64 signal_length);

    template <typename T>
    void QccWAVCDF97SynthesisSymmetricEvenEven( T* signal, Int64 signal_length);
};
};

#endif
