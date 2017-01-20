/* 
 *This program reads two raw data files,
 * and calculates Min, Max, RMSE, LMAX between the two.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define FLOAT float

void Evaluate2Arrays( const FLOAT* A, const FLOAT* B, size_t len, 
                      double* minmaxA, double* minmaxB, 
                      double* rms, double* nrmse, 
                      double* lmax, double* nlmax,
                      double* meanA, double* meanB )
{
    double sum = 0.0;
    double c = 0.0;
    double max = 0.0;
    double tmp;
    size_t i;
    double minA = A[0];
    double maxA = A[0];
    double minB = B[0];
    double maxB = B[0];
		double sum_A = 0.0;
		double sum_B = 0.0;
    for( i = 0; i < len; i++)
		{
				sum_A += A[i];	
				sum_B += B[i];	
        tmp = A[i] - B[i];
        if (tmp < 0)        tmp *= -1.0;
        if (tmp > max)      max = tmp;
        double y = tmp * tmp - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;

        /* Collect min, max */
        if( A[i] < minA )   minA = A[i];
        if( A[i] > maxA )   maxA = A[i];
        if( B[i] < minB )   minB = B[i];
        if( B[i] > maxB )   maxB = B[i];
    }
    sum /= (double)len;
    sum = sqrt( sum );

    *rms = sum;
    *lmax = max;
    minmaxA[0] = minA;
    minmaxA[1] = maxA;
    minmaxB[0] = minB;
    minmaxB[1] = maxB;

    *nrmse = sum / (maxA - minA);
    *nlmax = max / (maxA - minA);

		*meanA = sum_A / (double)len;
		*meanB = sum_B / (double)len;
}

int main (int argc, char* argv[] )
{
		if( argc != 3 )
		{
			printf("%s\n", "Usage: ./compareraw file1 file2");
			exit(0);
		}
		FILE* f1 = fopen (argv[1],"rb");
		fseek (f1, 0, SEEK_END);
		long size1 = ftell(f1);
		FILE* f2 = fopen (argv[2],"rb");
		fseek (f2, 0, SEEK_END);
		long size2 = ftell(f2);
		assert (size1 == size2);
		assert (size1 % 4 == 0);
		long nVals = size1 / 4;

		fseek( f1, 0, SEEK_SET );
		fseek( f2, 0, SEEK_SET );
		float* buf1 = (float*) malloc( size1 );
		float* buf2 = (float*) malloc( size2 );
		fread( buf1, sizeof(float), nVals, f1 );
		fread( buf2, sizeof(float), nVals, f2 );
		fclose( f1 );
		fclose( f2 );

    double minmaxA[2], minmaxB[2];
    double rms, nrms, lmax, nlmax, meanA, meanB;
    Evaluate2Arrays( buf1,buf2, nVals, minmaxA, minmaxB, 
                     &rms, &nrms, &lmax, &nlmax, &meanA, &meanB );
    printf("\t%s: min = %e, max = %e\n", argv[1], minmaxA[0], minmaxA[1] );
    printf("\t%s: min = %e, max = %e\n", argv[2], minmaxB[0], minmaxB[1] );
    printf("\tRMS = %e, LMAX = %e\n", rms, lmax );
    printf("\tNRMS = %e, NLMAX = %e\n", nrms, nlmax );
		printf("\tThe two files have mean values %.8e, %.8e.\n", meanA, meanB );
		printf("\tDifference of the two means: %.8e.\n", (meanA - meanB));

    free( buf1 );
    free( buf2 );

    return 0;
}
