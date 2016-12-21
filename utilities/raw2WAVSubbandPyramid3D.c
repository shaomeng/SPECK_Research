#include "stdio.h"
#include "stdlib.h"
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
  
  image_subband_pyramid.spatial_num_levels = 4;
  image_subband_pyramid.temporal_num_levels = 2;
  image_subband_pyramid.num_frames = NZ;
  image_subband_pyramid.num_rows = NY;
  image_subband_pyramid.num_cols = NX;
  QccWAVSubbandPyramid3DAlloc(&image_subband_pyramid);
}
