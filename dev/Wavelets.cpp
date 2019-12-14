#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iostream>

#include "Wavelets.h"

using namespace SPECK;

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::print_3D_data(T *data_3D, Int64 nFrame, Int64 nRow, Int64 nCol)
{
    Int64 i, j, k;
    printf("\n\n'");
    for(i=0; i< nFrame; i++)
    {
        printf("========================= Frame %ld ============================\n", i);
        for(j=0; j< nRow; j++)
        {
            for(k=0; k< nCol; k++)
                printf("%.4f    ", data_3D[i*nRow*nCol + j*nRow + k]);
            printf("\n");
        }
    }
}

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::QccWAVCDF97AnalysisSymmetricEvenEven( T* signal, Int64 signal_length)
{
    Int64 index;
    for (index = 1; index < signal_length - 2; index += 2)
        signal[index] += ALPHA * (signal[index - 1] + signal[index + 1]);

    signal[signal_length - 1] += 2 * ALPHA * signal[signal_length - 2];
    signal[0] += 2 * BETA * signal[1];

    for (index = 2; index < signal_length; index += 2)
        signal[index] += BETA * (signal[index + 1] + signal[index - 1]);

    for (index = 1; index < signal_length - 2; index += 2)
        signal[index] +=  GAMMA * (signal[index - 1] + signal[index + 1]);

    signal[signal_length - 1] += 2 * GAMMA * signal[signal_length - 2];
    signal[0] = EPSILON * (signal[0] + 2 * DELTA * signal[1]);

    for (index = 2; index < signal_length; index += 2)
        signal[index] =  EPSILON * (signal[index] + DELTA * (signal[index + 1] + signal[index - 1]));

    for (index = 1; index < signal_length; index += 2)
        signal[index] /= (-EPSILON);
}


/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::QccWAVCDF97AnalysisSymmetricEvenOdd( T* signal, Int64 signal_length)
{
  Int64 index;
  signal[0] += 2 * ALPHA * signal[1];

  for (index = 2; index < signal_length; index += 2)
    signal[index] += ALPHA * (signal[index - 1] + signal[index + 1]);
  
  for (index = 1; index < signal_length - 2; index += 2)
    signal[index] += BETA * (signal[index + 1] + signal[index - 1]);

  signal[signal_length - 1] += 2 * BETA * signal[signal_length - 2];
  signal[0] += 2 * GAMMA * signal[1];

  for (index = 2; index < signal_length; index += 2)
    signal[index] += GAMMA * (signal[index - 1] + signal[index + 1]);
  
  for (index = 1; index < signal_length - 2; index += 2)
    signal[index] = EPSILON * (signal[index] + DELTA * (signal[index + 1] + signal[index - 1]));

  signal[signal_length - 1] = EPSILON * (signal[signal_length - 1] + 2 * DELTA * signal[signal_length - 2]);
  
  for (index = 0; index < signal_length; index += 2)
    signal[index] /= (-EPSILON);
}

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::QccWAVCDF97SynthesisSymmetricEvenEven( T* signal, Int64 signal_length)
{
    Int64 index;
    for (index = 1; index < signal_length; index += 2)
        signal[index] *= (-EPSILON);

    signal[0] = signal[0]/EPSILON - 2 * DELTA * signal[1];

    for (index = 2; index < signal_length; index += 2)
        signal[index] = signal[index]/EPSILON - DELTA * (signal[index + 1] + signal[index - 1]);

    for (index = 1; index < signal_length - 2; index += 2)
        signal[index] -= GAMMA * (signal[index - 1] + signal[index + 1]);

    signal[signal_length - 1] -= 2 * GAMMA * signal[signal_length - 2];
    signal[0] -= 2 * BETA * signal[1];

    for (index = 2; index < signal_length; index += 2)
        signal[index] -= BETA * (signal[index + 1] + signal[index - 1]);

    for (index = 1; index < signal_length - 2; index += 2)
        signal[index] -= ALPHA * (signal[index - 1] + signal[index + 1]);

    signal[signal_length - 1] -= 2 * ALPHA * signal[signal_length - 2];
}

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::ForwardTransform1D( T* signal, Int64 length, Int64 nLevel )
{
    Int64 currentLength = length;
    T* buf = new T[ currentLength ];
    for( Int64 level = 0; level < nLevel; level++ )
    {
        std::memcpy( buf, signal, sizeof(T) * currentLength );

        QccWAVCDF97AnalysisSymmetricEvenEven( buf, currentLength );

        Int64 midPoint = currentLength / 2;
        for( Int64 i = 0; i < currentLength; i += 2 )
        {
            signal[ i/2 ]            = buf[ i   ];
            signal[ i/2 + midPoint ] = buf[ i+1 ];
        }

        currentLength /= 2;
    }
    delete[] buf;
}

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::InverseTransform1D( T* signal, Int64 length, Int64 nLevel )
{
    Int64 currentLength = length; 
    T* buf   = new T[ currentLength ];

    for( Int64 i = 1; i < nLevel; i++ )
        currentLength /= 2;

    for( Int64 level = nLevel; level > 0; level-- )
    {
        Int64 midPoint = currentLength / 2;
        for( Int64 i = 0; i < currentLength; i += 2 )
        {
            buf[i]   = signal[i/2];
            buf[i+1] = signal[i/2 + midPoint];
        }

        QccWAVCDF97SynthesisSymmetricEvenEven( buf, currentLength );

        std::memcpy( signal, buf, sizeof(T) * currentLength );

        currentLength *= 2;
    }
    delete[] buf;
}

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::ForwardTransform2D( T* signal, Int64 dim, Int64 nLevel )
{
    Int64 currentLength = dim;
    T* buf = new T[ currentLength ];

    for( Int64 level = 0; level < nLevel; level++ )
    {
        Int64 midPoint = currentLength / 2;

        /* transform every row */
        for( Int64 y = 0; y < currentLength; y++ )
        {
            T* startRow = signal + y * dim;
            std::memcpy( buf, startRow, sizeof(T) * currentLength );

            QccWAVCDF97AnalysisSymmetricEvenEven( buf, currentLength );

            for( Int64 x = 0; x < currentLength; x += 2 )
            {
                *(startRow + x / 2)             = buf[ x ];
                *(startRow + x / 2 + midPoint ) = buf[ x+1 ];
            }
        }

        /* transform every column */
        for( Int64 x = 0; x < currentLength; x++ )
        {
            for( Int64 y = 0; y < currentLength; y++ )
                buf[y] = signal[ y * dim + x ];

            QccWAVCDF97AnalysisSymmetricEvenEven( buf, currentLength );

            for( Int64 y = 0; y < currentLength; y += 2 )
            {
                signal[ (y/2) * dim + x ]            = buf[ y ];
                signal[ (y/2 + midPoint) * dim + x ] = buf[ y+1 ];
            }
        }

        currentLength /= 2;
    }
    delete[] buf;
}

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::InverseTransform2D( T* signal, Int64 dim, Int64 nLevel )
{
    Int64 currentLength = dim; 
    T* buf   = new T[ currentLength ];

    for( Int64 i = 1; i < nLevel; i++ )
        currentLength /= 2;

    for( Int64 level = nLevel; level > 0; level-- )
    {
        Int64 midPoint = currentLength / 2;

        /* inverse transform every column */
        for( Int64 x = 0; x < currentLength; x++ )
        {
            for( Int64 y = 0; y < currentLength; y += 2 )
            {
                buf[y]   = signal[y/2 * dim + x];
                buf[y+1] = signal[(y/2 + midPoint) * dim + x];
            }

            QccWAVCDF97SynthesisSymmetricEvenEven( buf, currentLength );

            for( Int64 y = 0; y < currentLength; y++ )
                signal[ y * dim + x ] = buf[y];
        }

        /* inverse transform every row */
        for( Int64 y = 0; y < currentLength; y++ )
        {
            T* startRow = signal + y * dim;
            for( Int64 x = 0; x < currentLength; x += 2 )
            {
                buf[x]   = *( startRow + x / 2 );
                buf[x+1] = *( startRow + x / 2 + midPoint );
            }

            QccWAVCDF97SynthesisSymmetricEvenEven( buf, currentLength );

            std::memcpy( signal + y * dim, buf, sizeof(T) * currentLength );
        }

        currentLength *= 2;
    }
    delete[] buf;
}

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::ForwardTransform3D( T*       signal, 
                                   Int64    xyDim, 
                                   Int64    zDim, 
                                   Int64    xyLevel, 
                                   Int64    zLevel )
{
    Int64 planeSize = xyDim * xyDim;

    /* transform zLevel in Z direction */
    T* buf = new T[zDim];
    for( Int64 y  = 0; y < xyDim; y++ )
        for( Int64 x = 0; x < xyDim; x++ )
        {
            T* startPos = signal + y * xyDim + x;
            for( Int64 z = 0; z < zDim; z++ )
                buf[z] = *(startPos + z * planeSize);

            ForwardTransform1D( buf, zDim, zLevel );

            for( Int64 z = 0; z < zDim; z++ )
                *(startPos + z * planeSize) = buf[z];
        }
    delete[] buf; 

    /* transform xyLevel in every XY plane */
    buf = new T[ planeSize ];
    for( Int64 z = 0; z < zDim; z++ )
    {
        T* startPos = signal + z * planeSize;
        std::memcpy( buf, startPos, sizeof(T) * planeSize );

        ForwardTransform2D( buf, xyDim, xyLevel );

        std::memcpy( startPos, buf, sizeof(T) * planeSize );
    }

    delete[] buf;
}

/*------------------------------------------------------------------------*/
template <typename T>
void Wavelets::InverseTransform3D( T*       signal, 
                                   Int64    xyDim, 
                                   Int64    zDim,  
                                   Int64    xyLevel,   
                                   Int64    zLevel )
{
    Int64 planeSize = xyDim * xyDim;

    /* inverse transform xyLevel in every XY plane */
    T* buf = new T[ planeSize ];
    for( Int64 z = 0; z < zDim; z++ )
    {
        T* startPos = signal + z * planeSize;
        std::memcpy( buf, startPos, sizeof(T) * planeSize );

        InverseTransform2D( buf, xyDim, xyLevel );

        std::memcpy( startPos, buf, sizeof(T) * planeSize );
    }

    delete[] buf;

    /* transform zLevel in Z direction */
    buf = new T[zDim];
    for( Int64 y  = 0; y < xyDim; y++ )
        for( Int64 x = 0; x < xyDim; x++ )
        {
            T* startPos = signal + y * xyDim + x;
            for( Int64 z = 0; z < zDim; z++ )
                buf[z] = *(startPos + z * planeSize);

            InverseTransform1D( buf, zDim, zLevel );

            for( Int64 z = 0; z < zDim; z++ )
                *(startPos + z * planeSize) = buf[z];
        }

    delete[] buf; 
}

int main()
{
    Int64 N = 32;
    Float32* signal = new Float32[ N * N * N ];
    Float32* orig   = new Float32[ N * N * N ];

    std::srand(std::time(nullptr)); 

    for( Int64 i = 0; i < N * N * N; i++ )
    {
        signal[i] = std::rand() % 10000 / 100.0;
        orig[i]   = signal[i];
    }

    Wavelets w;
    w.ForwardTransform3D( signal, N, N, 2, 1 );
    w.InverseTransform3D( signal, N, N, 2, 1 );
    for( Int64 i = 0; i < N*N*N; i++ )
        if( std::abs(orig[i] - signal[i]) > 0.0001 )
        {
            std::cout << "orig           signal" << std::endl;
            std::cout << orig[i] << "    " << signal[i] << std::endl;
        }

    delete[] signal;
    delete[] orig;
}

