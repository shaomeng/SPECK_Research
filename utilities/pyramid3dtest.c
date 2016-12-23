#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "libQccPack.h"

double GetRMSE2( const float *obs, const float *truth, long int n)
{
    double sum = 0.0;
    double c = 0.0;
    double diff;
    for( long int i = 0; i < n; i++) {
        diff = (obs[i] - truth[i]) ;
        double y = diff * diff - c;
        double t = sum + y;
        c = t - sum - y;
        sum = t;
    }   
    sum /= 1.0*n;
    
    return  sum;
}

double GetRMSE( const float *obs, const float *truth, long int n)
{
    double sum = GetRMSE2( obs, truth, n); 
    return sqrt(sum);
}

float GetMaxError( const float *arr1, const float *arr2, long int n)
{
    double diff = 0;
    double tmp;
    for( long int i = 0; i < n; i++) {
        tmp = arr1[i] - arr2[i];
        if( tmp < 0 )       tmp *= -1.0;
        if( tmp > diff)     diff = tmp;
    }

    return diff;
}

float GetMaxRelativeError( const float *obs, const float *truth, long int n)
{
    float relativeError = 0;
    float tmp;
    for( long int i = 0; i < n; i++) {
      if( truth[i] != 0.0 ) {
        tmp = (obs[i] - truth[i]) / truth[i];
        if( tmp < 0 )
           tmp *= -1.0;
        if( tmp > relativeError)
          relativeError = tmp;
      }
    }

    return relativeError;
}


int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		perror("Usage: ./raw2pyramid3d input_pyramid3d_file_(spbt)\n ");
		exit(1);
	}

	QccWAVSubbandPyramid3D subband_pyramid;
  QccWAVSubbandPyramid3DInitialize( &subband_pyramid );

  /* read input pyramid3d */
  QccConvertToQccString( subband_pyramid.filename, argv[1] );
  if( QccWAVSubbandPyramid3DRead( &subband_pyramid ) != 0 )
    perror("QccWAVSubbandPyramid3DRead() failed!\n");
  
  /* prepare wavelets */
  QccWAVWavelet Wavelet;
  // QccString WaveletFilename = QCCWAVWAVELET_DEFAULT_WAVELET;  /* CDF9/7 lifting scheme */
  QccString WaveletFilename = "CohenDaubechiesFeauveau.9-7.fbk";
  QccString Boundary = "symmetric";
  QccWAVWaveletInitialize(&Wavelet);
  if( QccWAVWaveletCreate(&Wavelet, WaveletFilename, Boundary) != 0 )
    perror("QccWAVWaveletCreate() failed!\n");
  int TransformType = QCCWAVSUBBANDPYRAMID3D_PACKET;  /* different levels in XY and Z */
  int temporal_num_levels = 2;
  int spatial_num_levels = 4;

  /* make a copy of input signal */
	QccWAVSubbandPyramid3D subband_pyramid_test;
  QccWAVSubbandPyramid3DInitialize( &subband_pyramid_test );
  if( QccWAVSubbandPyramid3DCopy( &subband_pyramid_test, &subband_pyramid ) != 0 )
    perror("QccWAVSubbandPyramid3DCopy() failed!\n");

  /* DWT and IDWT test */
  if( QccWAVSubbandPyramid3DDWT( &subband_pyramid_test, TransformType, temporal_num_levels,
                                  spatial_num_levels, &Wavelet ) != 0 )
    perror("QccWAVSubbandPyramid3DDWT() failed!\n");
  if( QccWAVSubbandPyramid3DInverseDWT( &subband_pyramid_test, &Wavelet ) != 0 )
    perror("QccWAVSubbandPyramid3DInverseDWT() failed!\n");

  /* debug info
   *
  perror("Printing original data:\n");
  QccWAVSubbandPyramid3DPrint( &subband_pyramid );
  perror("\nPrinting transformed data:\n");
  QccWAVSubbandPyramid3DPrint( &subband_pyramid_test );
   */

  /* compare results */
  long int nVals  = subband_pyramid.num_frames * subband_pyramid.num_rows * subband_pyramid.num_cols;
  float* orig_buf = (float*) malloc( sizeof(float) * nVals );
  float* test_buf = (float*) malloc( sizeof(float) * nVals );
  long frame, row, col;
  long counter = 0;
  for(    frame = 0; frame < subband_pyramid.num_frames; frame++ )
    for(    row = 0; row   < subband_pyramid.num_rows;   row++ )
      for(  col = 0; col   < subband_pyramid.num_cols;   col++ )
      {
        orig_buf[ counter ] = subband_pyramid.volume[frame][row][col];
        test_buf[ counter ] = subband_pyramid_test.volume[frame][row][col];
        counter++;
      } 

  double rmse             = GetRMSE( orig_buf, test_buf, nVals );
  float  maxError         = GetMaxError( orig_buf, test_buf, nVals );
  float  maxRelativeError = GetMaxRelativeError( test_buf, orig_buf, nVals );
  printf( "Test results:\n" );
  printf( "  RMSE               = %.10e \n", rmse );
  printf( "  maxError           = %.10e \n", maxError );
  printf( "  maxRelativeError   = %.10e \n", maxRelativeError );

  /* clean up */
  free( test_buf );
  free( orig_buf );
  QccWAVSubbandPyramid3DFree( &subband_pyramid_test );
  QccWAVWaveletFree(&Wavelet);
  QccWAVSubbandPyramid3DFree( &subband_pyramid );
}
