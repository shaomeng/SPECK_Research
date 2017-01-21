/*
 * This program takes in an image cube, applies wavelet transform,
 * thresholds the coefficients, applies inverse wavelet transform,
 * and then writes the result to disk.
 */

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "libQccPack.h"

int compare_float (const void *a, const void *b)
{
  // result in decending order
  const float *da = (const float *) a;
  const float *db = (const float *) b;

  return (*da < *db) - (*da > *db);
}

int main( int argc, char* argv[] )
{
	if( argc != 4 )
	{
		perror("Usage: ./imagecubeDwtCompression input_icb output_icb cratio\n ");
		exit(0);
	}

  int cratio = atoi( argv[3] );

  /* read input */
  QccIMGImageCube imagecube;
  QccIMGImageCubeInitialize( &imagecube );
  QccConvertToQccString( imagecube.filename, argv[1] );
  QccIMGImageCubeRead( &imagecube );

  /* create a QccWAVSubbandPyramid3D based on the input */
	QccWAVSubbandPyramid3D pyramid;
  QccWAVSubbandPyramid3DInitialize( &pyramid );
  pyramid.spatial_num_levels = 0;
  pyramid.temporal_num_levels = 0;
  pyramid.num_frames = imagecube.num_frames;
  pyramid.num_rows = imagecube.num_rows;
  pyramid.num_cols = imagecube.num_cols;
  QccWAVSubbandPyramid3DAlloc( &pyramid );
  QccVolumeCopy(  pyramid.volume, 
                  imagecube.volume,
                  pyramid.num_frames,
                  pyramid.num_rows,
                  pyramid.num_cols );

  /* prepare wavelets */
  QccWAVWavelet Wavelet;
  QccString WaveletFilename = QCCWAVWAVELET_DEFAULT_WAVELET;  /* CDF9/7 lifting */
  // QccString WaveletFilename = "CohenDaubechiesFeauveau.9-7.fbk";
  QccString Boundary = "symmetric";
  if( QccWAVWaveletInitialize(&Wavelet) != 0 )
    perror("QccWAVWaveletInitialize() failed!\n");
  if( QccWAVWaveletCreate(&Wavelet, WaveletFilename, Boundary) != 0 )
    perror("QccWAVWaveletCreate() failed!\n");
  int TransformType = QCCWAVSUBBANDPYRAMID3D_DYADIC;  /* same levels */
  // int TransformType = QCCWAVSUBBANDPYRAMID3D_PACKET;  /* different levels */
  int temporal_num_levels = 5;
  int spatial_num_levels  = 5;

  /* DWT */
  if( QccWAVSubbandPyramid3DDWT( &pyramid, 
                                  TransformType, 
                                  temporal_num_levels,
                                  spatial_num_levels, 
                                  &Wavelet ) != 0 )
    perror("QccWAVSubbandPyramid3DDWT() failed!\n");

  /* thresholding coeffs */
  if( cratio > 1 )
  {
    long nVals = pyramid.num_frames * pyramid.num_rows * pyramid.num_cols; 
    float* buf = (float*) malloc( nVals * 4 );
    long counter = 0;
    for( long frame = 0; frame < pyramid.num_frames; frame++ )
    for( long row   = 0; row   < pyramid.num_rows;   frame++ )
    for( long col   = 0; col   < pyramid.num_cols;   frame++ )
      buf[ counter++ ] = fabs( pyramid.volume[frame][row][col] );
    qsort( buf, nVals, sizeof(float), compare_float );
    float threshold = buf[ nVals / cratio ];
    free( buf );
    for( long frame = 0; frame < pyramid.num_frames; frame++ )
    for( long row   = 0; row   < pyramid.num_rows;   frame++ )
    for( long col   = 0; col   < pyramid.num_cols;   frame++ )
      if( fabs(pyramid.volume[frame][row][col]) < threshold )
        pyramid.volume[frame][row][col] = 0.0;
  }
      
  /* IDWT */
  if( QccWAVSubbandPyramid3DInverseDWT( &pyramid, &Wavelet ) != 0 )
    perror("QccWAVSubbandPyramid3DInverseDWT() failed!\n");

  /* Copy back to imagecube */
  QccVolumeCopy(  imagecube.volume,
                  pyramid.volume,
                  pyramid.num_frames,
                  pyramid.num_rows,
                  pyramid.num_cols );

  /* write out imagecube to a new name */
  QccConvertToQccString( imagecube.filename, argv[2] );
  QccIMGImageCubeWrite( &imagecube );

  QccWAVSubbandPyramid3DFree( &pyramid );
  QccIMGImageCubeFree( &imagecube );
}
