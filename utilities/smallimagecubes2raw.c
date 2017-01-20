/* 
 * This program reads in many image cubes in ICB format (e.g. 256^3) 
  * and put them together as a giant raw file (e.g. 1024^3).
 */
#include "libQccPack.h"
#include "assert.h"
#include "string.h"

/* input icb file dimensions */
#define BIGNX 1024
#define BIGNY 1024
#define BIGNZ 1024

/* output raw file dimensions */
#define SMALLNX 256
#define SMALLNY 256
#define SMALLNZ 256

int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		printf("%s\n", "Usage: ./smallcubes2raw filename.icb.4to1.cube0 .");
		printf("%s\n", "       (Only need to pass in the first cube in the sequence.)");
		exit(1);
	}

  // extract filename
	char inputName[256];
	int  inputSuffixLen = 1;
	strncpy( inputName, argv[1], strlen(argv[1]) - inputSuffixLen );		// discard the suffix
	inputName[ strlen(argv[1]) - inputSuffixLen ] = '\0';		// Sadly, strncpy doesn't do this automatically.
	// inputName looks like "filename.icb.4to1.cube" now.

	// allocate output buffer
	long nVals = BIGNX * BIGNY * BIGNZ;
	float* buf = (float*) malloc( nVals * 4 );  

  // split into smaller cubes
  long nCubesX = BIGNX / SMALLNX;
  long nCubesY = BIGNY / SMALLNY;
  long nCubesZ = BIGNZ / SMALLNZ;

	for( long cubeIdxZ = 0; cubeIdxZ < nCubesZ; cubeIdxZ++ )
	for( long cubeIdxY = 0; cubeIdxY < nCubesY; cubeIdxY++ )
	for( long cubeIdxX = 0; cubeIdxX < nCubesX; cubeIdxX++ )
	{
    long startX = cubeIdxX * SMALLNX;
    long startY = cubeIdxY * SMALLNY;
    long startZ = cubeIdxZ * SMALLNZ;
    long cubeIdx = cubeIdxZ * nCubesX * nCubesY + cubeIdxY * nCubesX + cubeIdxX;

    QccIMGImageCube imagecube;
    QccIMGImageCubeInitialize( &imagecube );
    QccStringSprintf( imagecube.filename, "%s%ld", inputName, cubeIdx );
	  printf("working on: %s\n", imagecube.filename );
		QccIMGImageCubeRead( &imagecube );

    for( long frame = 0; frame < imagecube.num_frames; frame++ )
    for( long row   = 0; row   < imagecube.num_rows;   row++ )
    for( long col   = 0; col   < imagecube.num_cols;   col++ )
    {
      float val = imagecube.volume[frame][row][col];
      buf[ (frame+startZ) * BIGNX * BIGNY + (row+startY) * BIGNX + (col+startX) ] = val;
    }

    QccIMGImageCubeFree(  &imagecube );   
	}

	char outputName[256];
	strcpy( outputName, argv[1] ); 
	char* icbLocation = strstr( outputName, "icb" );
	strncpy( icbLocation, "raw", 3 );
	// outputName looks like "filename.raw.4to1.cube0" now.
	char* cubeLocation = strstr( outputName, ".cube" );
	// outputName looks like "filename.raw.4to1" now.
	cubeLocation[0] = '\0';

	FILE* file = fopen( outputName, "wb" );
	fwrite( buf, sizeof(float), nVals, file );
	fclose( file ); 

	free (buf);
}
