#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "libQccPack.h"

#define NX 91
#define NY 91
#define NZ 30

int main( int argc, char* argv[] )
{
	if( argc != 3 )
	{
		perror("Usage: ./raw2WAVSubbandPyramid3D input_raw_file " \
					 "output_WAVSubbandPyramid3D_file");
		exit(1);
	}

	QccWAVSubbandPyramid3D image_subband_pyramid;
  QccWAVSubbandPyramid3DInitialize(&image_subband_pyramid);
  
  /* set metadata */
  QccConvertToQccString( image_subband_pyramid.filename, argv[2] );
  image_subband_pyramid.spatial_num_levels = 4;
  image_subband_pyramid.temporal_num_levels = 2;
  image_subband_pyramid.num_frames = NZ;
  image_subband_pyramid.num_rows = NY;
  image_subband_pyramid.num_cols = NX;
  QccWAVSubbandPyramid3DAlloc(&image_subband_pyramid);

  /* read raw data */
	FILE* f = fopen (argv[1], "rb");
	fseek (f, 0, SEEK_END);
	long nBytes = ftell(f);
	assert (nBytes % 6 == 0);
	assert (nBytes % 4 == 0);
  long nVals = nBytes / 4;
	assert (nVals == NX * NY * NZ);
	fseek( f, 0, SEEK_SET );
  float* buf = (float*)malloc( nBytes );
  fread( buf, sizeof(float), nVals, f );
  fclose( f );

  /* put values into QccWAVSubbandPyramid3D */
  long frame, row, col;
  long counter = 0;
  for(    frame = 0; frame < image_subband_pyramid.num_frames; frame++ )
    for(    row = 0; row   < image_subband_pyramid.num_rows;   row++ )
      for(  col = 0; col   < image_subband_pyramid.num_cols;   col++ )
      {
            image_subband_pyramid.volume[frame][row][col] = buf[ counter++ ];
      } 

  /* write QccWAVSubbandPyramid3D to disk */
  QccWAVSubbandPyramid3DWrite( &image_subband_pyramid );

  /* clean up */
  QccWAVSubbandPyramid3DFree( &image_subband_pyramid );
  free (buf);
}
