#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "libQccPack.h"

#define NX 91
#define NY 91
#define NZ 30

int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		perror("Usage: ./raw2pyramid3d input_pyramid3d_file_(spbt) ");
		exit(1);
	}

	QccWAVSubbandPyramid3D subband_pyramid;
  QccWAVSubbandPyramid3DInitialize( &subband_pyramid );

  /* read input pyramid3d */
  QccConvertToQccString( subband_pyramid.filename, argv[1] );
  QccWAVSubbandPyramid3DRead( &image_subband_pyramid );
  
  /* prepare wavelets */
}
