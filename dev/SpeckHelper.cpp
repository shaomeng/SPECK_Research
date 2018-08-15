#include "SpeckHelper.h" 

#include <iostream>
#include <cmath>
#include <limits>



/*------------------------------------------------------------------------*/
template< typename T  >
bool SpeckHelper::MakePositive( T*                   signal, 
                                Int64                length,     
                                std::vector<bool>&   positiveStateArray, 
                                Int32&               maxCoefficientBits )
{
    T maxMagnitude = -std::numeric_limits<T>::max();

    for( Int64 i = 0; i < length; i++ )
    {
        if( signal[i] < 0.0 )
        {
            positiveStateArray[i] = false;
            signal[i]             = -signal[i]; 
        }
        else
            positiveStateArray[i] = true;
            
        maxMagnitude = signal[i] > maxMagnitude ? signal[i] : maxMagnitude;
    }

    maxCoefficientBits = (Int32)std::floor(std::log2(maxMagnitude));

    return true;
}

template< typename T >
bool SpeckHelper::SubtractMean( T*    signal, 
                                Int64 length,
                                T&    mean )
{
	T sum = 0.0;
	T c   = 0.0;
	T y, t;
	for( Int64 i = 0; i < length; i++ )
	{
		y = signal[i] - c;
		t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}

	mean = sum / static_cast<T>(length);

	for( Int64 i = 0; i < length; i++ )
		signal[i] -= mean;

	return true;
}

