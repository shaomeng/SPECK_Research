#ifndef SPECKHELPER_H
#define SPECKHELPER_H

#include "Wavelets.h"
#include <vector>

using namespace SPECK;

class SpeckHelper
{
public:

    //
    // This function does 3 things:
    //   1. Make input "signal" to have all positive values
    //   2. Bookkeep the originally positive values as "true", and negative as "false"
    //   3. Report the highest bit plane index of the largest coefficient.
    //
    template< typename T  >
    bool MakePositive( T*                   signal, 
                       Int64                length,                 // signal length
                       std::vector<bool>&   positiveStateArray, 
                       Int32&               maxCoefficientBits );   // highest bit plane index

    //
    // This function takes in an input "signal," calculates its mean, 
    //   and suntract the mean from every data element.
    //
    template< typename T >
    bool SubtractMean( T*    signal, 
                       Int64 length,
                       T&    mean );
};

#endif
