/* 
 *This program reads two image cube data files,
 * and calculates Min, Max, RMSE, LMAX between the two.
 */

/*
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
*/
#include "libQccPack.h"

#define FLOAT float

void Evaluate2IMGcubes( const QccIMGImageCube* A, 
                        const QccIMGImageCube* B,
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
    double minA = MAXDOUBLE;
    double maxA = -MAXDOUBLE;
    double minB = MAXDOUBLE;
    double maxB = -MAXDOUBLE;
		double sum_A = 0.0;
		double sum_B = 0.0;
    for( long frame = 0; frame < A->num_frames; frame++ )
    for( long row   = 0;   row < A->num_rows;   row++ )
    for( long col   = 0;   col < A->num_cols;   col++ )
		{
				float Ai = A->volume[frame][row][col];	
				float Bi = B->volume[frame][row][col];	
				sum_A += Ai;
				sum_B += Bi;
        tmp = fabs(Ai - Bi);
        if (tmp > max)      max = tmp;
        double y = tmp * tmp - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;

        /* Collect min, max */
        if( Ai < minA )   minA = Ai;
        if( Ai > maxA )   maxA = Ai;
        if( Bi < minB )   minB = Bi;
        if( Bi > maxB )   maxB = Bi;
    }
    long len = A->num_frames * A->num_rows * A->num_cols;
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
			printf("%s\n", "Usage: ./compareimagecube file1 file2");
			exit(0);
		}
    QccIMGImageCube cube1, cube2;
    QccIMGImageCubeInitialize( &cube1 );
    QccIMGImageCubeInitialize( &cube2 );
    QccConvertToQccString( cube1.filename, argv[1] );
    QccConvertToQccString( cube2.filename, argv[2] );
		QccIMGImageCubeRead( &cube1 );
		QccIMGImageCubeRead( &cube2 );

    double minmaxA[2], minmaxB[2];
    double rms, nrms, lmax, nlmax, meanA, meanB;
    Evaluate2IMGcubes( &cube1, &cube2, minmaxA, minmaxB, 
                     &rms, &nrms, &lmax, &nlmax, &meanA, &meanB );
    printf("\t%s: min = %e, max = %e\n", argv[1], minmaxA[0], minmaxA[1] );
    printf("\t%s: min = %e, max = %e\n", argv[2], minmaxB[0], minmaxB[1] );
    printf("\tRMS = %e, LMAX = %e\n", rms, lmax );
    printf("\tNRMS = %e, NLMAX = %e\n", nrms, nlmax );
		printf("\tThe two files have mean values %.8e, %.8e.\n", meanA, meanB );
		printf("\tDifference of the two means: %.8e.\n", (meanA - meanB));

		QccIMGImageCubeFree( &cube1 );
		QccIMGImageCubeFree( &cube2 );

    return 0;
}
