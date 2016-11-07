#include <string>
#include <iostream>

// https://github.com/DigitalInBlue/Celero
#include "celero/Celero.h"

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
            results[++idx] = ch;
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


auto remove_ctrl_cstyle(char *output, const char *input, const size_t N) {
    for (size_t idx = 0; idx < N; ++idx) {
        if (input[idx] >= 0x20) {
            *output++ = input[idx];
        }
        *output = 0;
    }
}

constexpr int NumberOfSamples = 100;
constexpr int NumberOfIterations = 100;

auto const sdata = read("tOptimizeString.cpp");
std::string sresults(sdata.size(), 0);
const std::vector<char> vdata = convert(sdata);
std::vector<char> vresults(vdata.size());
char *output = new char[sdata.size() + 1];

CELERO_MAIN

BASELINE(remove_ctrl, basic, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl(sdata);
}

// BENCHMARK(remove_ctrl, empty, NumberOfSamples, NumberOfIterations) {}

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

BENCHMARK(remove_ctrl, cstyle, NumberOfSamples, NumberOfIterations) {
    remove_ctrl_cstyle(output, &vdata[0], vdata.size());
}
