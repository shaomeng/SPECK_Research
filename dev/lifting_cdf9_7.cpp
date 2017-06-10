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
void ForwardTransform1D( Float64* signal, Int64 length, Int64 nLevel )
{
	Int64 currentLength = length;
	for( Int64 level = 0; level < nLevel; level++ )
	{
		Float64* buf = new Float64[ currentLength ];
    std::memcpy( buf, signal, sizeof(Float64) * currentLength );

		QccWAVCDF97AnalysisSymmetricEvenEven( buf, currentLength );

		Int64 midPoint = currentLength / 2;
		for( Int64 i = 0; i < currentLength; i += 2 )
		{
			signal[ i/2 ]            = buf[i];
			signal[ i/2 + midPoint ] = buf[ i+1 ];
		}
		
    delete[] buf;
		currentLength /= 2;
	}
}

/*------------------------------------------------------------------------*/
void InverseTransform1D( Float64* signal, Int64 length, Int64 nLevel )
{
	Int64 currentLength = length; 
	for( Int64 i = 1; i < nLevel; i++ )
		currentLength /= 2;

	for( Int64 level = nLevel; level > 0; level-- )
	{
		Int64 midPoint = currentLength / 2;
		Float64* buf   = new Float64[ currentLength ];
		for( Int64 i = 0; i < currentLength; i += 2 )
		{
			buf[i]   = signal[i/2];
			buf[i+1] = signal[i/2 + midPoint];
		}

		QccWAVCDF97SynthesisSymmetricEvenEven( buf, currentLength );

    std::memcpy( signal, buf, sizeof(Float64) * currentLength );

    delete[] buf;
		currentLength *= 2;
	}
}


/*------------------------------------------------------------------------*/
void ForwardTransform2D( Float64* signal, Int64 dim, Int64 nLevel )
{
	Int64 currentLength = dim;

	for( Int64 level = 0; level < nLevel; level++ )
	{
    Float64* buf = new Float64[ currentLength ];

    /* transform every row */
    for( Int64 y = 0; y < currentLength; y++ )
    {
      Float64* currentStart = signal + y * dim;
      std::memcpy( buf, currentStart, sizeof(Float64) * currentLength );

      QccWAVCDF97AnalysisSymmetricEvenEven( buf, currentLength );

      Int64 midPoint = currentLength / 2;
      for( Int64 x = 0; x < currentLength; x += 2 )
      {
        signal[ y * dim + x/2 ]            = buf[x];
        signal[ y * dim + x/2 + midPoint ] = buf[ x+1 ];
      }
    }

    /* transform every column */
    for( Int64 x = 0; x < currentLength; x++ )
    {
      for( Int64 y = 0; y < currentLength; y++ )
        buf[y] = signal[ y * dim + x ];

      QccWAVCDF97AnalysisSymmetricEvenEven( buf, currentLength );

      Int64 midPoint = currentLength / 2;
      for( Int64 y = 0; y < currentLength; y += 2 )
      {
        signal[ (y/2) * dim + x ]            = buf[y];
        signal[ (y/2 + midPoint) * dim + x ] = buf[ y+1 ];
      }
    }

    delete[] buf;
    currentLength /= 2;
	}
}

int main()
{
	Int64 N = 16;
	Float64* signal = new Float64[ N * N ];
	for( Int64 i = 0; i < N * N; i++ )
		signal[i] = i * 0.1;

	ForwardTransform2D( signal, N, 1 );
	print_2D_data( signal, N, N );

	delete[] signal;
}
