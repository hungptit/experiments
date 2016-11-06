# Introduction #

# A simple function #

# Use mutating operator #

# Why do we need to reserve memory if possible #

# Pass by reference or const reference #

# Use iterator #

# Use range for loop #

# Use std::for_each #

# Use std::vector<char> instead of std::string if possible #

# C style function #

# How to bear C style function #

# Final performance results #

    Celero
    Timer resolution: 0.001000 us
    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |      4003.60000 |          249.78 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.00830 |        33.23000 |        30093.29 | 
    remove_ctrl     | reserve         | Null            |             100 |             100 |         0.01213 |        48.57000 |        20588.84 | 
    remove_ctrl     | ref_args        | Null            |             100 |             100 |         0.00740 |        29.62000 |        33760.97 | 
    remove_ctrl     | ref_args_it     | Null            |             100 |             100 |         0.00743 |        29.73000 |        33636.06 | 
    remove_ctrl     | ref_args_cit    | Null            |             100 |             100 |         0.00743 |        29.74000 |        33624.75 | 
    remove_ctrl     | range_for       | Null            |             100 |             100 |         0.00504 |        20.17000 |        49578.58 | 
    remove_ctrl     | for_each        | Null            |             100 |             100 |         0.00501 |        20.04000 |        49900.20 | 
    remove_ctrl     | vector          | Null            |             100 |             100 |         0.00253 |        10.11000 |        98911.97 | 
    remove_ctrl     | vector_revise   | Null            |             100 |             100 |         0.00217 |         8.67000 |       115340.25 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00247 |         9.90000 |       101010.10 | 
    Complete.

# Conclusion #
* A carefully designed C++ API can be at as fast a C style function.
* C style function is the 2nd in term of performance, however, the C array is not mathematically correct. We should avoid of this coding style if possible.
* std::vector<char> is significantly faster than std::string.


