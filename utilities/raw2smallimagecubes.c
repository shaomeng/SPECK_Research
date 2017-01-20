/* 
 * This program breaks down a big raw file (e.g. 1024^3) to smaller
 * QccIMGImageCube structures (e.g. 256^3).
 */
#include "libQccPack.h"
#include "assert.h"
#include "string.h"

/* input raw file dimensions */
#define INNX 1024
#define INNY 1024
#define INNZ 1024

/* target cube dimensions */
#define OUTNX 256
#define OUTNY 256
#define OUTNZ 256

int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		printf("%s\n", "Usage: ./raw2smallimagecubes input_file.");
		exit(1);
	}

  // verify size
	char inputName[256];
	int  inputSuffixLen = 3;
	strncpy( inputName, argv[1], strlen(argv[1]) - inputSuffixLen );		// discard the suffix
	inputName[ strlen(argv[1]) - inputSuffixLen ] = '\0';		// Sadly, strncpy doesn't do this automatically.
	FILE* f = fopen (argv[1],"rb");
	fseek (f, 0, SEEK_END);
	long size = ftell(f);
	assert (size % 4 == 0);
	long nVals = size / 4;
	assert ( nVals == INNX * INNY * INNZ );

  // read out raw data
	fseek( f, 0, SEEK_SET );
	float* buf = (float*) malloc( nVals * 4 );  
	fread( buf, sizeof(float), nVals, f );
	fclose( f );

  // split into smaller cubes
  long nCubesX = INNX / OUTNX;
  long nCubesY = INNY / OUTNY;
  long nCubesZ = INNZ / OUTNZ;

	for( long cubeIdxZ = 0; cubeIdxZ < nCubesZ; cubeIdxZ++ )
	for( long cubeIdxY = 0; cubeIdxY < nCubesY; cubeIdxY++ )
	for( long cubeIdxX = 0; cubeIdxX < nCubesX; cubeIdxX++ )
	{
    long startX = cubeIdxX * OUTNX;
    long startY = cubeIdxY * OUTNY;
    long startZ = cubeIdxZ * OUTNZ;
    long cubeIdx = cubeIdxZ * nCubesX * nCubesY + cubeIdxY * nCubesX + cubeIdxX;

    QccIMGImageCube imagecube;
    QccIMGImageCubeInitialize( &imagecube );
    QccStringSprintf( imagecube.filename, "%sicb.cube%ld", inputName, cubeIdx );
	  printf("working on: %s\n", imagecube.filename );

    imagecube.num_cols = OUTNX;
    imagecube.num_rows = OUTNY;
    imagecube.num_frames = OUTNZ;
    if( QccIMGImageCubeAlloc( &imagecube ) )
        QccErrorPrintMessages();
    float min = MAXFLOAT;
    float max = -MAXFLOAT;

    for( long frame = 0; frame < imagecube.num_frames; frame++ )
    for( long row   = 0; row   < imagecube.num_rows;   row++ )
    for( long col   = 0; col   < imagecube.num_cols;   col++ )
    {
      float val = buf[ (frame+startZ) * INNX * INNY + (row+startY) * INNX + (col+startX) ];
      if( val < min )
        min = val;
      if( val > max )
        max = val;
      imagecube.volume[frame][row][col] = val;
    }

    imagecube.min_val = min;
    imagecube.max_val = max;    

    QccIMGImageCubeWrite( &imagecube );
    QccIMGImageCubeFree(  &imagecube );   
	}

	free (buf);
}
