How to optimize simple string function for performance.
====================

# Goals #

* Demonstrate steps that I used to optimize a simple string conversion function using:
    * C++11/14.
    * Generic programming concepts.
    * STL library
    * Others
* Give advices about how to improve your C++ code using the modern C++.

# Experimental design #

Compiler: I use clang-4.0.0 compiler with -O4 flag to compile all examples. The test data is the content of a binary file (65992 bytes). Below is the code for preparing test data

    std::string read(const std::string &dataFile) {
        boost::iostreams::mapped_file mmap(dataFile, boost::iostreams::mapped_file::readonly);
        auto begin = mmap.const_data();
        auto end = begin + mmap.size();
        return std::string(begin, end);
    }
    
    std::vector<char> convert(const std::string &s) {
        return std::vector<char>(s.cbegin(), s.cend());
    }


    auto const sdata = read("tOptimizeString.cpp");
    std::string sresults(sdata.size(), 0);
    const std::vector<char> vdata = convert(sdata);
    std::vector<char> vresults(vdata.size());
    char *output = new char[sdata.size() + 1];

    constexpr int NumberOfSamples = 100;
    constexpr int NumberOfIterations = 100;

# A simple C++ function #

Let first start with a very basic implentation of a function that will remove control charactor from a string.

    std::string remove_ctrl(std::string s) {
        std::string results;
        for (size_t idx = 0; idx < s.length(); ++idx) {
            if (s[idx] >= 0x20) {
                results = results + s[idx];
            }
        }
        return results;
    }

# A similar function written in C style #

    auto remove_ctrl_cstyle(char *output, const char *input, const size_t N) {
        for (size_t idx = 0; idx < N; ++idx) {
            if (input[idx] >= 0x20) {
                *output++ = input[idx];
            }
            *output = 0;
        }
    }

Performance results:

    Celero
    Timer resolution: 0.001000 us
    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.96000 |         1462.07 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00418 |         2.86000 |       349650.35 |


# Use mutating operator #

Our simple implentation has several issues, however, I will improve one issue at a time then compare the performance results with the previous version so that we can see improvement that we have made during the optimization process step by step. In the previous implementation, we can clearly see that **results = results + s[idx]** i the our performance bottle neck since we create a temporary string for every single iteration.

    std::string remove_ctrl_mutating(std::string s) {
        std::string results;
        for (size_t idx = 0; idx < s.length(); ++idx) {
            if (s[idx] >= 0x20) {
                results += s[idx];
            }
        }
        return results;
    }

Performance results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.82000 |         1462.37 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00442 |         3.02000 |       331125.83 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.02134 |        14.59000 |        68540.10 |


# We need to reserve memory of the output if possible #

We have already improved the performance of of our code using mutating operator. However, it might be faster if we can reserve the memory of **results** to avoid of performing string resize inside the for loop.

    std::string remove_ctrl_reserve(std::string s) {
        std::string results;
        results.reserve(s.length());
        for (size_t idx = 0; idx < s.length(); ++idx) {
            if (s[idx] >= 0x20) {
                results += s[idx];
            }
        }
        return results;
    }

Performance results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.82000 |         1462.37 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00442 |         3.02000 |       331125.83 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.02134 |        14.59000 |        68540.10 | 
    remove_ctrl     | reserve         | Null            |             100 |             100 |         0.02273 |        15.54000 |        64350.06 |


# Pass by const reference #

Note that we are passing a string **s** by value and the string copy constructor will be called every time we call remove_ctrl_reserve function. Let make it better by using a constant reference.

    std::string remove_ctrl_ref_args(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        for (size_t idx = 0; idx < s.length(); ++idx) {
            if (s[idx] >= 0x20) {
                results += s[idx];
            }
        }
        return results;
    }

Performance results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.82000 |         1462.37 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00442 |         3.02000 |       331125.83 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.02134 |        14.59000 |        68540.10 | 
    remove_ctrl     | reserve         | Null            |             100 |             100 |         0.02273 |        15.54000 |        64350.06 | 
    remove_ctrl     | ref_args        | Null            |             100 |             100 |         0.01977 |        13.52000 |        73964.50 |


# Use iterator #

Now our function is in a very good shape. Can we do a better job? Yes we can make it a little better by using iterator.

    std::string remove_ctrl_ref_args_it(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        for (auto it = s.begin(); it != s.end(); ++it) {
            if (*it >= 0x20) {
                results += *it;
            }
        }
        return results;
    }

Performance results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.82000 |         1462.37 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00442 |         3.02000 |       331125.83 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.02134 |        14.59000 |        68540.10 | 
    remove_ctrl     | reserve         | Null            |             100 |             100 |         0.02273 |        15.54000 |        64350.06 | 
    remove_ctrl     | ref_args        | Null            |             100 |             100 |         0.01977 |        13.52000 |        73964.50 | 
    remove_ctrl     | ref_args_it     | Null            |             100 |             100 |         0.01821 |        12.45000 |        80321.29 |


# Use const iterator #

We know that using const_iterator might be faster than iterator since we do not modify our input data. Let convert iterator to const_iterator and check that we can improve the performance of our function.

std::string remove_ctrl_ref_args_cit(const std::string &s) {
    std::string results;
    results.reserve(s.length());
    for (auto it = s.cbegin(); it != s.cend(); ++it) {
        if (*it >= 0x20) {
            results += *it;
        }
    }
    return results;
}

Performace results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.82000 |         1462.37 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00442 |         3.02000 |       331125.83 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.02134 |        14.59000 |        68540.10 | 
    remove_ctrl     | reserve         | Null            |             100 |             100 |         0.02273 |        15.54000 |        64350.06 | 
    remove_ctrl     | ref_args        | Null            |             100 |             100 |         0.01977 |        13.52000 |        73964.50 | 
    remove_ctrl     | ref_args_it     | Null            |             100 |             100 |         0.01821 |        12.45000 |        80321.29 | 
    remove_ctrl     | ref_args_cit    | Null            |             100 |             100 |         0.01790 |        12.24000 |        81699.35 |


# Use range for loop #

Range for loop is one of the most useful feature of C++11. Let use range for-loop instead of const_iterator to see if we can improve the performance of our code.

    std::string remove_ctrl_range_for(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        for (auto ch : s) {
            if (ch >= 0x20) {
                results += ch;
            }
        }
        return results;
    }

Performance results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.82000 |         1462.37 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00442 |         3.02000 |       331125.83 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.02134 |        14.59000 |        68540.10 | 
    remove_ctrl     | reserve         | Null            |             100 |             100 |         0.02273 |        15.54000 |        64350.06 | 
    remove_ctrl     | ref_args        | Null            |             100 |             100 |         0.01977 |        13.52000 |        73964.50 | 
    remove_ctrl     | ref_args_it     | Null            |             100 |             100 |         0.01821 |        12.45000 |        80321.29 | 
    remove_ctrl     | ref_args_cit    | Null            |             100 |             100 |         0.01790 |        12.24000 |        81699.35 | 
    remove_ctrl     | range_for       | Null            |             100 |             100 |         0.01644 |        11.24000 |        88967.97 |


# Use std::for_each #

STL also provide some functions that allow us to iterate over a given range of data. These functions should be as fast as that of the hand written code, however, it is easier to read or the perpose of the for-loop will be stand out by using these function.

    std::string remove_ctrl_for_each(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        std::for_each(s.cbegin(), s.cend(), [&results](auto ch) {
            if (ch >= 0x20) {
                results += ch;
            }
        });
        return results;
    }

Performance results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.82000 |         1462.37 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00442 |         3.02000 |       331125.83 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.02134 |        14.59000 |        68540.10 | 
    remove_ctrl     | reserve         | Null            |             100 |             100 |         0.02273 |        15.54000 |        64350.06 | 
    remove_ctrl     | ref_args        | Null            |             100 |             100 |         0.01977 |        13.52000 |        73964.50 | 
    remove_ctrl     | ref_args_it     | Null            |             100 |             100 |         0.01821 |        12.45000 |        80321.29 | 
    remove_ctrl     | ref_args_cit    | Null            |             100 |             100 |         0.01790 |        12.24000 |        81699.35 | 
    remove_ctrl     | range_for       | Null            |             100 |             100 |         0.01644 |        11.24000 |        88967.97 | 
    remove_ctrl     | for_each        | Null            |             100 |             100 |         0.01642 |        11.23000 |        89047.20 |


# Use std::vector<char> instead of std::string if possible #

It is wellknown that using std::vector<char> is faster than std::string. Let convert our input parameter to std::vector<char> and verify that we can improve the performance of our existing code.

    auto remove_ctrl_vector(const std::vector<char> &s) {
        std::vector<char> results;
        results.reserve(s.size());
        auto idx = 0;
        for (auto ch : s) {
            if (ch >= 0x20) {
                results[idx++] = ch;
            }
        }
        return results;
    }

Performance results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |             100 |             100 |         1.00000 |       683.82000 |         1462.37 | 
    remove_ctrl     | cstyle          | Null            |             100 |             100 |         0.00442 |         3.02000 |       331125.83 | 
    remove_ctrl     | mutating        | Null            |             100 |             100 |         0.02134 |        14.59000 |        68540.10 | 
    remove_ctrl     | reserve         | Null            |             100 |             100 |         0.02273 |        15.54000 |        64350.06 | 
    remove_ctrl     | ref_args        | Null            |             100 |             100 |         0.01977 |        13.52000 |        73964.50 | 
    remove_ctrl     | ref_args_it     | Null            |             100 |             100 |         0.01821 |        12.45000 |        80321.29 | 
    remove_ctrl     | ref_args_cit    | Null            |             100 |             100 |         0.01790 |        12.24000 |        81699.35 | 
    remove_ctrl     | range_for       | Null            |             100 |             100 |         0.01644 |        11.24000 |        88967.97 | 
    remove_ctrl     | for_each        | Null            |             100 |             100 |         0.01642 |        11.23000 |        89047.20 | 
    remove_ctrl     | vector          | Null            |             100 |             100 |         0.00543 |         3.71000 |       269541.78 |


# Move memory allocation out of our function #

We call reserve function to preallocate memory for the output results in previous functions. If we move this logic out of our function then we will gain 3% improvement. However, our C++ function is still slower then the C style function.

    template <typename InContainer, typename OutContainer>
    auto remove_ctrl_vector_revise1(InContainer &data, OutContainer &results) {
        size_t idx = 0;
        for (auto const ch : data) {
            if (ch >= 0x20) {
                results[++idx] = ch;
            }
        }
        return idx;
    }


Performance results:

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |              50 |              10 |         1.00000 |     22966.30000 |           43.54 | 
    remove_ctrl     | cstyle          | Null            |              50 |              10 |         0.00334 |        76.60000 |        13054.83 | 
    remove_ctrl     | mutating        | Null            |              50 |              10 |         0.00826 |       189.60000 |         5274.26 | 
    remove_ctrl     | reserve         | Null            |              50 |              10 |         0.00823 |       188.90000 |         5293.81 | 
    remove_ctrl     | ref_args        | Null            |              50 |              10 |         0.00720 |       165.40000 |         6045.95 | 
    remove_ctrl     | ref_args_it     | Null            |              50 |              10 |         0.00641 |       147.30000 |         6788.87 | 
    remove_ctrl     | ref_args_cit    | Null            |              50 |              10 |         0.00668 |       153.40000 |         6518.90 | 
    remove_ctrl     | range_for       | Null            |              50 |              10 |         0.00588 |       135.00000 |         7407.41 | 
    remove_ctrl     | for_each        | Null            |              50 |              10 |         0.00603 |       138.40000 |         7225.43 | 
    remove_ctrl     | vector          | Null            |              50 |              10 |         0.00386 |        88.60000 |        11286.68 | 
    remove_ctrl     | vector_revise   | Null            |              50 |              10 |         0.00375 |        86.10000 |        11614.40 |

# Final results #

Obtained using clang-4.0.0

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |              50 |              10 |         1.00000 |     24140.20000 |           41.42 | 
    remove_ctrl     | cstyle          | Null            |              50 |              10 |         0.00317 |        76.60000 |        13054.83 | 
    remove_ctrl     | mutating        | Null            |              50 |              10 |         0.00785 |       189.40000 |         5279.83 | 
    remove_ctrl     | reserve         | Null            |              50 |              10 |         0.00783 |       188.90000 |         5293.81 | 
    remove_ctrl     | ref_args        | Null            |              50 |              10 |         0.00679 |       163.80000 |         6105.01 | 
    remove_ctrl     | ref_args_it     | Null            |              50 |              10 |         0.00610 |       147.20000 |         6793.48 | 
    remove_ctrl     | ref_args_cit    | Null            |              50 |              10 |         0.00616 |       148.70000 |         6724.95 | 
    remove_ctrl     | range_for       | Null            |              50 |              10 |         0.00558 |       134.70000 |         7423.90 | 
    remove_ctrl     | for_each        | Null            |              50 |              10 |         0.00574 |       138.60000 |         7215.01 | 
    remove_ctrl     | vector          | Null            |              50 |              10 |         0.00367 |        88.60000 |        11286.68 | 
    remove_ctrl     | vector_revise   | Null            |              50 |              10 |         0.00355 |        85.80000 |        11655.01 | 
    remove_ctrl     | vector_revise1  | Null            |              50 |              10 |         0.00367 |        88.50000 |        11299.44 |


Obtained using gcc-4.9.2

    -----------------------------------------------------------------------------------------------------------------------------------------------
         Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  | 
    -----------------------------------------------------------------------------------------------------------------------------------------------
    remove_ctrl     | basic           | Null            |              50 |              10 |         1.00000 |     22835.30000 |           43.79 | 
    remove_ctrl     | cstyle          | Null            |              50 |              10 |         0.00416 |        95.10000 |        10515.25 | 
    remove_ctrl     | mutating        | Null            |              50 |              10 |         0.00834 |       190.40000 |         5252.10 | 
    remove_ctrl     | reserve         | Null            |              50 |              10 |         0.00718 |       164.00000 |         6097.56 | 
    remove_ctrl     | ref_args        | Null            |              50 |              10 |         0.00598 |       136.50000 |         7326.01 | 
    remove_ctrl     | ref_args_it     | Null            |              50 |              10 |         0.00588 |       134.30000 |         7446.02 | 
    remove_ctrl     | ref_args_cit    | Null            |              50 |              10 |         0.00572 |       130.70000 |         7651.11 | 
    remove_ctrl     | range_for       | Null            |              50 |              10 |         0.00531 |       121.20000 |         8250.83 | 
    remove_ctrl     | for_each        | Null            |              50 |              10 |         0.00538 |       122.90000 |         8136.70 | 
    remove_ctrl     | vector          | Null            |              50 |              10 |         0.00393 |        89.70000 |        11148.27 | 
    remove_ctrl     | vector_revise   | Null            |              50 |              10 |         0.00411 |        93.80000 |        10660.98 | 
    remove_ctrl     | vector_revise1  | Null            |              50 |              10 |         0.00394 |        89.90000 |        11123.47 |


# Source code #

    #include <string>
    #include <iostream>
    
    // https://github.com/DigitalInBlue/Celero
    #include "celero/Celero.h"
    
    // For file I/O
    #include "boost/iostreams/device/mapped_file.hpp"
    
    std::string read(const std::string &dataFile) {
        boost::iostreams::mapped_file mmap(dataFile, boost::iostreams::mapped_file::readonly);
        auto begin = mmap.const_data();
        auto end = begin + mmap.size();
        return std::string(begin, end);
    }
    
    std::vector<char> convert(const std::string &s) {
        return std::vector<char>(s.cbegin(), s.cend());
    }
    
    std::string remove_ctrl(std::string s) {
        std::string results;
        for (size_t idx = 0; idx < s.length(); ++idx) {
            if (s[idx] >= 0x20) {
                results = results + s[idx];
            }
        }
        return results;
    }
    
    std::string remove_ctrl_mutating(std::string s) {
        std::string results;
        for (size_t idx = 0; idx < s.length(); ++idx) {
            if (s[idx] >= 0x20) {
                results += s[idx];
            }
        }
        return results;
    }
    
    std::string remove_ctrl_reserve(std::string s) {
        std::string results;
        results.reserve(s.length());
        for (size_t idx = 0; idx < s.length(); ++idx) {
            if (s[idx] >= 0x20) {
                results += s[idx];
            }
        }
        return results;
    }
    
    std::string remove_ctrl_ref_args(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        for (size_t idx = 0; idx < s.length(); ++idx) {
            if (s[idx] >= 0x20) {
                results += s[idx];
            }
        }
        return results;
    }
    
    std::string remove_ctrl_ref_args_it(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        for (auto it = s.begin(); it != s.end(); ++it) {
            if (*it >= 0x20) {
                results += *it;
            }
        }
        return results;
    }
    
    std::string remove_ctrl_ref_args_cit(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        for (auto it = s.cbegin(); it != s.cend(); ++it) {
            if (*it >= 0x20) {
                results += *it;
            }
        }
        return results;
    }
    
    std::string remove_ctrl_range_for(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        for (auto ch : s) {
            if (ch >= 0x20) {
                results += ch;
            }
        }
        return results;
    }
    
    std::string remove_ctrl_for_each(const std::string &s) {
        std::string results;
        results.reserve(s.length());
        std::for_each(s.cbegin(), s.cend(), [&results](auto ch) {
            if (ch >= 0x20) {
                results += ch;
            }
        });
        return results;
    }
    
    auto remove_ctrl_vector(const std::vector<char> &s) {
        std::vector<char> results;
        results.reserve(s.size());
        auto idx = 0;
        for (auto ch : s) {
            if (ch >= 0x20) {
                results[idx++] = ch;
            }
        }
        return results;
    }
    
    auto remove_ctrl_vector_revise(std::vector<char>::const_iterator ibegin, std::vector<char>::const_iterator iend, std::vector<char>::iterator obegin) {
        auto oit = obegin;
        for (auto it = ibegin; it != iend; ++it) {
            if (*it >= 0x20) {
              *(++oit) = *it;;
            }
        }
        return oit;
    }
    
    // template <typename InputIterator, typename OutputIterator>
    // auto remove_ctrl_vector_revise(InputIterator ibegin, InputIterator iend, OutputIterator obegin) {
    //     auto oit = obegin;
    //     for (auto it = ibegin; it != iend; ++it) {
    //         if (*it >= 0x20) {
    //           *(++oit) = *it;;
    //         }
    //     }
    //     return oit;
    // }
    
    template <typename InContainer, typename OutContainer>
    auto remove_ctrl_vector_revise1(InContainer &data, OutContainer &results) {
        size_t idx = 0;
        for (auto const ch : data) {
            if (ch >= 0x20) {
                results[++idx] = ch;
            }
        }
        return idx;
    }
    
    auto remove_ctrl_cstyle(char *output, const char *input, const size_t N) {
        for (size_t idx = 0; idx < N; ++idx) {
            if (input[idx] >= 0x20) {
                *output++ = input[idx];
            }
            *output = 0;
        }
    }
    
    auto const sdata = read("tTuplevsStruct");
    std::string sresults(sdata.size(), 0);
    const std::vector<char> vdata = convert(sdata);
    std::vector<char> vresults(vdata.size());
    char *output = new char[sdata.size() + 1];
    
    constexpr int NumberOfSamples = 50;
    constexpr int NumberOfIterations = 10;
    
    
    CELERO_MAIN
    
    BASELINE(remove_ctrl, basic, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl(sdata);
    }
    
    BENCHMARK(remove_ctrl, cstyle, NumberOfSamples, NumberOfIterations) {
        remove_ctrl_cstyle(output, &vdata[0], vdata.size());
    }
    
    BENCHMARK(remove_ctrl, mutating, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl_mutating(sdata);
    }
    
    BENCHMARK(remove_ctrl, reserve, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl_reserve(sdata);
    }
    
    BENCHMARK(remove_ctrl, ref_args, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl_ref_args(sdata);
    }
    
    BENCHMARK(remove_ctrl, ref_args_it, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl_ref_args_it(sdata);
    }
    
    BENCHMARK(remove_ctrl, ref_args_cit, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl_ref_args_cit(sdata);
    }
    
    BENCHMARK(remove_ctrl, range_for, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl_range_for(sdata);
    }
    
    BENCHMARK(remove_ctrl, for_each, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl_for_each(sdata);
    }
    
    BENCHMARK(remove_ctrl, vector, NumberOfSamples, NumberOfIterations) {
        auto results = remove_ctrl_vector(vdata);
    }
    
    BENCHMARK(remove_ctrl, vector_revise, NumberOfSamples, NumberOfIterations) {
      remove_ctrl_vector_revise(vdata.cbegin(), vdata.cend(), vresults.begin());
    }
    
    BENCHMARK(remove_ctrl, vector_revise1, NumberOfSamples, NumberOfIterations) {
        remove_ctrl_vector_revise1(vdata, vresults);
    }


# Conclusion #
* A carefully designed C++ API can be at as fast a C style function.
* C style memory model is not mathematically correct. We should avoid of this coding style if possible.
* std::vector<char> is significantly faster than std::string for storing a string buffer.
* Minimize resource allocation when designing your shared API for performance.
