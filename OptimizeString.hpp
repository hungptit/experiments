
#include <iostream>
#include <string>
#include <vector>

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

auto remove_ctrl_vector_revise(std::vector<char>::const_iterator ibegin,
                               std::vector<char>::const_iterator iend,
                               std::vector<char>::iterator obegin) {
    auto oit = obegin;
    for (auto it = ibegin; it != iend; ++it) {
        if (*it >= 0x20) {
            *(oit) = *it;
            ++oit;
        }
    }
    return oit;
}

// template <typename InputIterator, typename OutputIterator>
// auto remove_ctrl_vector_revise(InputIterator ibegin, InputIterator iend, OutputIterator
// obegin) {
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
            results[idx] = ch;
            ++idx;
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
