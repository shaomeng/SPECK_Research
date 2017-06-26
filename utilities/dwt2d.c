#include "libQccPack.h"

int main( int argc, char** argv )
{
	int NX = 8;
	int NY = 8;
	int nLevel = 1;

	/* allocate memory for a 1D array */
	float* buf = (float*)malloc( NX * NY * 4 );
	for( int i = 0; i < NX * NY; i++ )
		buf[i] = i * 0.1;

	/* allocate data structure to take in this 1D array */
	QccWAVSubbandPyramid 		coeff2d;
	QccWAVSubbandPyramidInitialize( &coeff2d );
	coeff2d.num_cols = NX;
	coeff2d.num_rows = NY;
	QccWAVSubbandPyramidAlloc( &coeff2d );
	int counter = 0;
	for( int row = 0; row < NY; row++ )
		for( int col = 0; col < NX; col++ )
			coeff2d.matrix[row][col] = buf[ counter++ ];
	QccWAVSubbandPyramidPrint( &coeff2d );


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


	/* forward transform */
	QccWAVSubbandPyramidDWT( &coeff2d, nLevel, &cdf97 );
	QccWAVSubbandPyramidPrint( &coeff2d );


	/* inverse transform */
	QccWAVSubbandPyramidInverseDWT( &coeff2d, &cdf97 );
	QccWAVSubbandPyramidPrint( &coeff2d );

	/* free memory */
	free( buf );
	QccWAVSubbandPyramidFree( &coeff2d );
}
