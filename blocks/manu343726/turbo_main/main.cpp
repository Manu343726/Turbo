
#include "manu343726/turbo_core/turbo_core.hpp"

#include <iostream>

using m = tml::maps::map<tml::maps::pair<int          , unsigned int>      ,
                         tml::maps::pair<long int     , unsigned long int> ,
                         tml::maps::pair<long long int, unsigned long long int>
                        >;


template<typename T>
using unsigned_type = tml::maps::at<m,T>;

int main()
{
    std::cout << "Signed to unsigned map: " << tml::to_string<m>() << std::endl;
	std::cout << tml::to_string<long int>() << " --> " << tml::to_string<unsigned_type<long int>>() << std::endl;	
}