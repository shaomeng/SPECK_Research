#include <cstdio>
#include <cstdlib>
#include <cstring>


#define ALPHA     -1.58615986717275
#define BETA      -0.05297864003258
#define GAMMA      0.88293362717904
#define DELTA      0.44350482244527
#define EPSILON    1.14960430535816
#define M_SQRT2    1.41421356237309504880168872420969808   /* sqrt(2)        */

#define QCCWAVWAVELET_PHASE_ODD     1
#define QCCWAVWAVELET_PHASE_EVEN    0


/*------------------------------------------------------------------------*/
template< typename T>
void printSignal( T *signal, long signal_length){
	long index;
	for(index = 0; index < signal_length; index = index + 1)
		printf("%g ", signal[index]);
	printf("\n\n");
}

/*------------------------------------------------------------------------*/
int check_correct(const float* arr1, const float* arr2, unsigned int len){
	unsigned int i;
	float epsilon = 0.0000000001f;
	for(i=0; i<len; i++)
		if(std::abs(arr1[i]-arr2[i])>epsilon) 
            return 0;
	return 1;
}


/*------------------------------------------------------------------------*/
void copyArr(float *dest_arr, float *source_arr, unsigned int len){
	unsigned int i;
	for(i=0; i<len; i++) dest_arr[i] = source_arr[i];
}

/*------------------------------------------------------------------------*/
void errorMessage(const char *format){
    printf("\n%s\n",format);
}
/*------------------------------------------------------------------------*/
void transpose_2D(float *src_arr, float *dst_arr, int nRow, int nCol){
    int index = 0;
    for (int i = 0; i < nRow; i++)
        for (int j = 0; j < nCol; j++){
            dst_arr[index] = src_arr[i + j*nRow];
            index++;
        }
}
/*------------------------------------------------------------------------*/
template< typename T >
void print_2D_data(T *data_2D, long nX, long nY){
    for(long i=0; i< nY; i++){
        for(long j=0; j< nX; j++)
            printf("% 10.4f ", data_2D[i*nX + j]);
        printf("\n");
    }
    printf("===============================\n");
}
/*------------------------------------------------------------------------*/
template< typename T >
void transpose_3D(T *src_arr, T *dst_arr, long size, const int orientation){
    long i,j,k, src_index=0, dst_index=0;
    register long stride_x, stride_y, stride_z;
    switch (orientation){
        case 1:/* XYZ --> XZY */
            stride_x = size;
            stride_y = size*size;
            stride_z = 1;
            break;
        case 2:/* XYZ --> YXZ */
            stride_x = size*size;
            stride_y = 1;
            stride_z = size;
            break;
        case 3:/* XYZ --> YZX */
            stride_x = 1;
            stride_y = size*size;
            stride_z = size;
            break;
        case 4:/* XYZ --> ZXY */
            stride_x = size;
            stride_y = 1;
            stride_z = size*size;
            break;
        case 5:/* XYZ --> ZYX */
            stride_x = 1;
            stride_y = size;
            stride_z = size*size;
            break;
    }
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            for(k=0; k<size; k++){
                src_index = i*stride_x + j*stride_y + k*stride_z;
                dst_arr[dst_index] = src_arr[src_index];
                dst_index = dst_index + 1;
            }
}

/*------------------------------------------------------------------------*/
template <typename T>
void print_3D_data(T *data_3D, long nFrame, long nRow, long nCol){
    long i, j, k;
    printf("\n\n'");
    for(i=0; i< nFrame; i++){
        printf("===================================== Frame %ld ====================================\n", i);
        for(j=0; j< nRow; j++){
            for(k=0; k< nCol; k++)
                printf("%.4f    ", data_3D[i*nRow*nCol + j*nRow + k]);
            printf("\n");
        }
    }
}

/*------------------------------------------------------------------------*/
template <typename T>
void InPlaceTranspose_XYZ_2_ZYX( T* volume, long dim )	// volume has size dim^3
{
	long srcX, srcY, srcZ, srcIdx, dstIdx;
	long planeSize = dim * dim;

	/* 2D transpose on every de facto XZ plane */
	for( srcZ = 0; srcZ < dim; srcZ++ )
		// pragma here
		for( srcY = 0; srcY < dim; srcY++ )
			for( srcX = srcZ + 1; srcX < dim;  srcX++ )
			{
				srcIdx = srcZ * planeSize + srcY * dim + srcX;
				dstIdx = srcX * planeSize + srcY * dim + srcZ;
				T tmp  					 = volume[ srcIdx ];
				volume[ srcIdx ] = volume[ dstIdx ];
				volume[ dstIdx ] = tmp;
			}
}

/*------------------------------------------------------------------------*/
template <typename T>
void InPlaceTranspose_ZYX_2_YZX( T* volume, long dim )	// volume has size dim^3
{
	long srcX, srcY, srcZ, srcIdx, dstIdx;
	long planeSize = dim * dim;

	/* 2D transpose on every de facto XY plane */
	for( srcZ = 0; srcZ < dim; srcZ++ )
		// pragma here
		for( srcY = 0; srcY < dim; srcY++ )
			for( srcX = srcY + 1; srcX < dim;  srcX++ )
			{
				srcIdx = srcZ * planeSize + srcY * dim + srcX;
				dstIdx = srcX * planeSize + srcY * dim + srcZ;
				T tmp  					 = volume[ srcIdx ];
				volume[ srcIdx ] = volume[ dstIdx ];
				volume[ dstIdx ] = tmp;
			}
}

