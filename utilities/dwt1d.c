#include "libQccPack.h"

int main( int argc, char** argv )
{
	int N = 16;

	/* generate input buffer */
	QccVector orig = QccVectorAlloc( N );
	for( int i = 0; i < N; i++ )
		orig[i] = i+10.0;
	printf("data array before transform: \n");
	QccVectorPrint( orig, N );

	/* create a wavelet */
	QccWAVWavelet cdf97;
	QccString WaveletFilename = "CohenDaubechiesFeauveau.9-7.lft";
	QccString Boundary = "symmetric";
	QccWAVWaveletInitialize( &cdf97 );
  if (QccWAVWaveletCreate( &cdf97, WaveletFilename, Boundary ))
	{
		QccErrorAddMessage("%s: Error calling QccWAVWaveletCreate()", argv[0]);
		QccErrorExit();
	}

	int nLevel = 1;

	/* forward wavelet transform, 1 level */
	QccVector coeff_1_level = QccVectorAlloc( N ); /* create a buffer to store coefficients */
	QccVectorCopy( coeff_1_level, orig, N );
	
	QccWAVWaveletDWT1D( coeff_1_level, N, 0, 0, nLevel, &cdf97 );
	printf("\ncoefficients from 1 level of transform: \n");
	QccVectorPrint( coeff_1_level, N );

	/* inverse wavelet transform, 1 level */
	QccWAVWaveletInverseDWT1D( coeff_1_level, N, 0, 0, nLevel, &cdf97 );
	printf("\ndata array recovered from 1 level of transform: \n");
	QccVectorPrint( coeff_1_level, N );

	/* free memory */
	QccVectorFree( orig );
	QccVectorFree( coeff_1_level );
}
