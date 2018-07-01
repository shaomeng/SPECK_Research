#include "Wavelets.h"

#include<vector>

using namespace SPECK;

class SpeckHelper
{
public:
    //
    // This function does 3 things:
    //   1. Make input "signal" to have all positive values
    //   2. Bookkeep the originally positive values as "true", and negative as "false"
    //   3. Bookkeep the maximum magnitude observed.
    template< typename T  >
    bool MakePositive( T*                   signal, 
                       Int64                length,     // signal length
                       std::vector<bool>&   positiveStateArray, 
                       Float64&             maxMagnitude );

}



