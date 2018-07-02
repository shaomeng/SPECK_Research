#include "SpeckHelper.h" 

#include <iostream>
#include <cmath>
#include <limits>



/*------------------------------------------------------------------------*/
template< typename T  >
bool SpeckHelper::MakePositive( T*                   signal, 
                                Int64                length,     
                                std::vector<bool>&   positiveStateArray, 
                                Int32&               maxCoefficientBits )
{
    T maxMagnitude = -std::numeric_limits<T>::max();

    for( Int64 i = 0; i < length; i++ )
    {
        if( signal[i] < 0.0 )
        {
            positiveStateArray[i] = false;
            signal[i]             = -signal[i]; 
        }
        else
            positiveStateArray[i] = true;
            
        maxMagnitude = signal[i] > maxMagnitude ? signal[i] : maxMagnitude;
    }

    maxCoefficientBits = (Int32)std::floor(std::log2(maxMagnitude));

    return true;
}

template< typename T >
bool SpeckHelper::SubtractMean( T*    signal, 
                                Int64 length,
                                T&    mean )
{
	T sum = 0.0;
	T c   = 0.0;
	T y, t;
	for( Int64 i = 0; i < length; i++ )
	{
		y = signal[i] - c;
		t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}

	mean = sum / static_cast<T>(length);

	for( Int64 i = 0; i < length; i++ )
		signal[i] -= mean;

	return true;
}

#if 0
//
// QccSPECK3DEncode()
//
template< typename T >
Float64 speckEncode3D( T* signal,                   // Input
                       QccBitBuffer* output_buffer,  // Output
                       Int64 NX,        
                       Int64 NY,   
                       Int64 NZ, 
                       Int64 xyLevel,  
                       Int64 zLevel,
                       Int64 target_bit_cnt )
{
  QccENTArithmeticModel *model = NULL;
  std::vector<bool> state_array;
  Float64 image_mean  = 0.0;
  Int64 max_coefficient_bits;
  Float64 threshold;
  Int64 frame, row, col;
  QccList LIS, LSP;
  QccListInitialize(&LIS);
  QccListInitialize(&LSP);
  Int64 bitplane = 0;
  
  //
  // QccSPECK3DEncodeDWT()
  // subtractMean ->  DWT -> make coeffs positive
  //
  image_mean = subtractMean( signal, NX * NY * NZ );
  ForwardTransform3D( signal, NX, NY, NZ, xyLevel, zLevel );
  Float64 maxCoeff = makePositive( signal, state_array, NX * NY * NZ ); 
  max_coefficient_bits = static_cast<Int64>(floor( log2(maxCoeff) ));

  
  QccSPECK3DEncodeHeader( output_buffer,
                          QCCWAVSUBBANDPYRAMID3D_PACKET,
                          zLevel,
                          xyLevel,
                          NZ,
                          NY,
                          NX,
                          image_mean,
                          max_coefficient_bits );
  
  model = QccENTArithmeticEncodeStart( QccSPECK3DNumSymbols,
                                       QCCSPECK3D_NUM_CONTEXTS,
                                       NULL,
                                       target_bit_cnt );

  threshold = pow( 2.0, max_coefficient_bits );

  return 0.0;
}








int main()
{
	Int64 N = 3;
	Float64* signal = new Float64[ N * N * N ];
	for( Int64 i = 0; i < N * N * N; i++ )
		signal[i] = i * 0.1 - 1.0;
	
  std::vector<bool> state_array;

	makePositive( signal, state_array, N*N*N );
  Float64 mean = subtractMean( signal, N*N*N );

  std::cout << "Mean = " << mean << std::endl;
	for( Int64 i = 0; i < N*N*N; i++ )
		std::cout << state_array[i] << ", " << signal[i] << std::endl;

	delete[] signal;


  QccBitBuffer outputBuffer;
  outputBuffer.type = QCCBITBUFFER_OUTPUT;
  QccBitBufferStart(&outputBuffer);
}
#endif
