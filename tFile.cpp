#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "Timer.hpp"
#include "fmt/format.h"

constexpr size_t LEN = 32;

template <typename T> std::vector<T> create_test_data(const size_t N) {
    std::vector<T> data(N);
    for (size_t idx = 0; idx < N; ++idx) {
        data[idx] = idx;
    }
    return data;
}

// A modified version of the original approach.
void first_approach(const std::vector<double> &data, const std::string &fileName) {
    utils::ElapsedTime<utils::MILLISECOND> t("Original approach: ", true);
    std::ofstream fout(fileName);
    for (size_t l = 0; l < data.size(); l++) {
        fout << std::setprecision(10) << data.at(l) << std::endl;
    }
    fout.close();
}

// Cache to a string stream before writing to the output file
void second_approach(const std::vector<double> &data, const std::string &fileName) {
    utils::ElapsedTime<utils::MILLISECOND> t("stringstream: ", true);

    // Serialize data to the string first.
    std::stringstream buffer;
    for (const double value : data) {
        buffer << std::setprecision(10) << value << "\n";
    }

    // Now write to the output file.
    std::ofstream fout(fileName);
    fout << buffer.str();
    fout.close();
}

// Use sprintf
void third_approach(const std::vector<double> &data, const std::string &fileName) {
    utils::ElapsedTime<utils::MILLISECOND> t("sprintf: ", true);

    // Serialize data to the string first.
    std::stringstream buffer;
    char aLine[LEN];
    std::for_each(data.cbegin(), data.cend(), [&buffer, &aLine](const double value) {
        sprintf(aLine, "%.10g\n", value);
        buffer << aLine;
    });

    // Now write to the output file.
    std::ofstream fout(fileName);
    fout << buffer.str();
    fout.close();
}

// Use fmt::MemoryWriter (https://github.com/fmtlib/fmt)
void fourth_approach(const std::vector<double> &data, const std::string &fileName) {
    utils::ElapsedTime<utils::MILLISECOND> t("Use fmt: ", true);
    // Serialize data to the string first.
    std::stringstream buffer;
    char aLine[LEN];
    fmt::MemoryWriter writer;
    std::for_each(data.cbegin(), data.cend(), [&aLine, &writer](const double value) {
        sprintf(aLine, "%.10g\n", value);
        writer << aLine;
    });

    // Now write to the output file.
    std::ofstream fout(fileName);
    fout << writer.str();
    fout.close();
}

// Use std::vector<char>
void fifth_approach(const std::vector<double> &data, const std::string &fileName) {
    utils::ElapsedTime<utils::MILLISECOND> t("std::vector<char>: ", true);
    // Serialize data to the string first.
    char aLine[LEN];
    std::vector<char> buffer;
    buffer.reserve(data.size() * 10);
    std::for_each(data.cbegin(), data.cend(), [&buffer, &aLine](const double value) {
        sprintf(aLine, "%.10g\n", value);
        const size_t len = strlen(aLine);
        for (size_t idx = 0; idx < len; ++idx) {
            buffer.push_back(aLine[idx]);
        }
    });

    // Now write to the output file.
    std::ofstream fout(fileName);
    fout << buffer.data();
    fout.close();
}

int main() {
    constexpr size_t N = 3000000;
    std::vector<double> data = create_test_data<double>(N);
    first_approach(data, "first.txt");
    second_approach(data, "second.txt");
    third_approach(data, "third.txt");
    fourth_approach(data, "fourth.txt");
    fifth_approach(data, "fifth.txt");
}
