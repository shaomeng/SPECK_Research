#include "common.h"

/* the following is true on most intel machines */
using Float64 = double;		
using Float32 = float;		
using Int64 = long;
using Int32 = int;

/*------------------------------------------------------------------------*/
void QccWAVCDF97AnalysisSymmetricEvenEven( Float64* signal, Int64 signal_length)
{
  Int64 index;
  for (index = 1; index < signal_length - 2; index += 2)
    signal[index] += ALPHA * (signal[index - 1] + signal[index + 1]);

  signal[signal_length - 1] += 2 * ALPHA * signal[signal_length - 2];
  signal[0] += 2 * BETA * signal[1];

  for (index = 2; index < signal_length; index += 2)
    signal[index] += BETA * (signal[index + 1] + signal[index - 1]);
  
  for (index = 1; index < signal_length - 2; index += 2)
    signal[index] +=  GAMMA * (signal[index - 1] + signal[index + 1]);

  signal[signal_length - 1] += 2 * GAMMA * signal[signal_length - 2];
  signal[0] = EPSILON * (signal[0] + 2 * DELTA * signal[1]);

  for (index = 2; index < signal_length; index += 2)
    signal[index] =  EPSILON * (signal[index] + DELTA * (signal[index + 1] + signal[index - 1]));
  
  for (index = 1; index < signal_length; index += 2)
    signal[index] /= (-EPSILON);
}

/*------------------------------------------------------------------------*/
void QccWAVCDF97SynthesisSymmetricEvenEven( Float64* signal, Int64 signal_length)
{
    Int64 index;
    for (index = 1; index < signal_length; index += 2)
        signal[index] *= (-EPSILON);

    signal[0] = signal[0]/EPSILON - 2 * DELTA * signal[1];

    for (index = 2; index < signal_length; index += 2)
        signal[index] = signal[index]/EPSILON - DELTA * (signal[index + 1] + signal[index - 1]);
    
    for (index = 1; index < signal_length - 2; index += 2)
        signal[index] -= GAMMA * (signal[index - 1] + signal[index + 1]);

    signal[signal_length - 1] -= 2 * GAMMA * signal[signal_length - 2];
    signal[0] -= 2 * BETA * signal[1];

    for (index = 2; index < signal_length; index += 2)
        signal[index] -= BETA * (signal[index + 1] + signal[index - 1]);
    
    for (index = 1; index < signal_length - 2; index += 2)
        signal[index] -= ALPHA * (signal[index - 1] + signal[index + 1]);

    signal[signal_length - 1] -= 2 * ALPHA * signal[signal_length - 2];
}

/*------------------------------------------------------------------------*/
template <typename T>
void ForwardTransform1D( T* signal, Int64 length, Int64 nLevel )
{
	Int64 currentLength = length;
	for( Int64 level = 0; level < nLevel; level++ )
	{
		Float64 buf[ currentLength ];
		for( Int64 i = 0; i < currentLength; i++ )
			buf[i] = static_cast<Float64>( signal[i] );

		QccWAVCDF97AnalysisSymmetricEvenEven( buf, currentLength );

		Int64 midPoint = currentLength / 2;
		for( Int64 i = 0; i < currentLength; i += 2 )
		{
			signal[ i/2 ] = static_cast<T>(buf[i]);
			signal[ i/2 + midPoint ] = static_cast<T>(buf[ i+1 ]);
		}
		
		currentLength /= 2;
	}
}

/*------------------------------------------------------------------------*/
template <typename T>
void InverseTransform1D( T* signal, Int64 length, Int64 nLevel )
{
	Int64 currentLength = length; 
	for( Int64 i = 1; i < nLevel; i++ )
		currentLength /= 2;

	for( Int64 level = nLevel; level > 0; level-- )
	{
		Int64 midPoint = currentLength / 2;
		Float64 buf[ currentLength ];
		for( Int64 i = 0; i < currentLength; i += 2 )
		{
			buf[i]   = static_cast<Float64>( signal[i/2] );
			buf[i+1] = static_cast<Float64>( signal[i/2 + midPoint] );
		}

		QccWAVCDF97SynthesisSymmetricEvenEven( buf, currentLength );

		for( Int64 i = 0; i < currentLength; i++ )
			signal[i] = static_cast<T>( buf[i] );

		currentLength *= 2;
	}
}


int main()
{
	Int64 N = 256;
	Float64* signal = new Float64[ N ];
	for( Int64 i = 0; i < N; i++ )
		signal[i] = i * 0.1;

	ForwardTransform1D( signal, N, 5 );
	printSignal( signal, N );
	InverseTransform1D( signal, N, 5 );
	printSignal( signal, N );

	delete[] signal;
}
