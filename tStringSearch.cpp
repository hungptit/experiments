#include <string>
#include <iostream>
#include "boost/algorithm/searching/knuth_morris_pratt.hpp"

int main() {
    std::string searchString = "Hello world worl";
    std::string pattern = "world1";    
    using iter_type = decltype(pattern.begin());
    boost::algorithm::knuth_morris_pratt<iter_type> alg(pattern.begin(), pattern.end());
    auto val = alg(searchString.cbegin(), searchString.cend());
    if (val != searchString.end()) {
        std::cout << "Found \"" << pattern << "\" in \"" << searchString << "\"\n";
    } else {
        std::cout << "Cannot find \"" << pattern << "\" in \"" << searchString << "\"\n";
    }
}
