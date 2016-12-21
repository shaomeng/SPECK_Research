#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>

#define NX 91
#define NY 91
#define NZ 30

int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		std::cerr << "Usage: ./raw2faces.cpp input_file" << std::endl;
		exit(1);
	}

  // verify size
	std::string inFilename = argv[1];
	FILE* f = fopen (argv[1],"rb");
	fseek (f, 0, SEEK_END);
	long size = ftell(f);
	assert (size % 6 == 0);
	assert (size % 4 == 0);

  // read out raw data
	fseek( f, 0, SEEK_SET );
	long nVals = size / 4;
	float* buf = new float[ nVals ];
	fread( buf, sizeof(float), nVals, f );
	fclose( f );

  // split into 6 files
	for( long face = 0; face < 6; face++ )
	{
		long offset = face * NX * NY * NZ;
		std::string outFilename = inFilename + ".face" + std::to_string(face);
		f = fopen( outFilename.c_str(), "wb" );
		fwrite( buf + offset, sizeof(float), NX * NY * NZ, f );
		fclose( f );
	}

	delete[] buf;
}
