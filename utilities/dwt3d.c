#include "libQccPack.h"

int main( int argc, char** argv )
{
	int NX = 256;
	int NY = 256;
	int NZ = 256;
	int nLevel = 5;

	/* allocate memory for a 1D array */
	float* buf = (float*)malloc( NX * NY * NZ * 4 );
	//for( int i = 0; i < NX * NY * NZ; i++ )
	//	buf[i] = i * 0.1;

  // read in file tacc-turbulence-256-volume.raw
  FILE* f  = fopen( "tacc-turbulence-256-volume.raw", "r" );
  fread( buf, 4, NX*NY*NZ, f );
  fclose( f );

	/* allocate data structure to take in this 1D array */
	QccWAVSubbandPyramid3D 		coeff3d;
	QccWAVSubbandPyramid3DInitialize( &coeff3d );
	coeff3d.num_cols = NX;
	coeff3d.num_rows = NY;
	coeff3d.num_frames = NZ;
	QccWAVSubbandPyramid3DAlloc( &coeff3d );
	int counter = 0;
	for( int frame = 0; frame < NZ; frame++ )
		for( int row = 0; row < NY; row++ )
			for( int col = 0; col < NX; col++ )
				coeff3d.volume[frame][row][col] = buf[ counter++ ];
	//QccWAVSubbandPyramid3DPrint( &coeff3d );


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
	QccWAVSubbandPyramid3DDWT( &coeff3d, QCCWAVSUBBANDPYRAMID3D_PACKET, 
															nLevel, nLevel, &cdf97 );
	//QccWAVSubbandPyramid3DPrint( &coeff3d );


	/* inverse transform */
	QccWAVSubbandPyramid3DInverseDWT( &coeff3d, &cdf97 );
	//QccWAVSubbandPyramid3DPrint( &coeff3d );

  // copy result to a buffer
  counter = 0;
	for( int frame = 0; frame < NZ; frame++ )
		for( int row = 0; row < NY; row++ )
			for( int col = 0; col < NX; col++ )
				buf[ counter++ ] = (float)coeff3d.volume[frame][row][col];

  // write result to qccpack_result.raw
  f = fopen( "qccpack_result.raw", "w" );
  fwrite( buf, 4, NX*NY*NZ, f );
  fclose( f );

	/* free memory */
	free( buf );
	QccWAVSubbandPyramid3DFree( &coeff3d );
}
