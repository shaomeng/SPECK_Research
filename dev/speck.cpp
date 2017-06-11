#include "speck.h" 

using namespace wavelet;

template< typename T  >
void makePositive( T* signal, std::vector<bool> &positiveStateArray, Int64 length )
{
	/* assign positiveStateArray to have length "trues" */
	positiveStateArray.assign( length, true );

	for( Int64 i = 0; i < length; i++ )
		if( signal[i] < 0 )
		{
			positiveStateArray[i] = false;
			signal[i] *= -1.0;
		}
}

template< typename T >
Float64 subtractMean( T* signal, Int64 length )
{
	Float64 sum = 0.0;
	Float64 c   = 0.0;
	Float64 y, t;
	for( Int64 i = 0; i < length; i++ )
	{
		y = signal[i] - c;
		t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}

	Float64 mean = sum / static_cast<Float64>(length);

	for( Int64 i = 0; i < length; i++ )
		signal[i] -= mean;

	return mean;
}


int main()
{
	Int64 N = 3;
	Float64* signal = new Float64[ N * N * N ];
	for( Int64 i = 0; i < N * N * N; i++ )
		signal[i] = i * 0.1 - 1.0;
	
  std::vector<bool> state_array;

	makePositive( signal, state_array, N*N*N );

	for( Int64 i = 0; i < N*N*N; i++ )
		std::cout << state_array[i] << ", " << signal[i] << std::endl;

	delete[] signal;
}
