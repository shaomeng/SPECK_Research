/* 
 * This program reads in one image cube in ICB format 
  * and put them together as a raw file.
 */
#include "libQccPack.h"
#include "assert.h"
#include "string.h"

int main( int argc, char* argv[] )
{
	if( argc != 3 )
	{
		printf("%s\n", "Usage: ./imagecube2raw input_file.icb output_file.raw .");
		exit(1);
	}

  QccIMGImageCube imagecube;
  QccIMGImageCubeInitialize( &imagecube );
  QccStringSprintf( imagecube.filename, "%s", argv[1] );
  // printf("reading  %s\n", imagecube.filename );
  QccIMGImageCubeRead( &imagecube );
  // printf("finish reading \n" );

	// allocate output buffer
	long NX = imagecube.num_cols;
	long NY = imagecube.num_rows;
	long NZ = imagecube.num_frames;
	long nVals = NX * NY * NZ;
	float* buf = (float*) malloc( nVals * 4 );  

  for( long frame = 0; frame < NZ; frame++ )
  	for( long row   = 0; row   < NY;   row++ )
  		for( long col   = 0; col   < NX;   col++ )
			{
				float val = imagecube.volume[frame][row][col];
				buf[ (frame) * NX * NY + (row) * NX + (col) ] = val;
			}

  QccIMGImageCubeFree(  &imagecube );   

	// printf("Output file: %s\n", argv[2] );
	FILE* file = fopen( argv[2], "wb" );
	fwrite( buf, sizeof(float), nVals, file );
	fclose( file ); 

	free (buf);
}
