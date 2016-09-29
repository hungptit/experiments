#pragma once

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>

#include "Timer.hpp"
#include "fmt/format.h"

#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

namespace {
    constexpr size_t LEN = 32;

    template <typename T> std::vector<T> create_test_data(const size_t N) {
        std::vector<T> data(N);
        for (size_t idx = 0; idx < N; ++idx) {
            data[idx] = idx;
        }
        return data;
    }

    template <typename Iterator> auto toVectorOfChar(Iterator begin, Iterator end) {
        char aLine[LEN];
        std::vector<char> buffer;
        buffer.reserve(std::distance(begin, end) * LEN);
        std::for_each(begin, end, [&buffer, &aLine](const auto value) {
            sprintf(aLine, "%g\n", value);
            const size_t len = strlen(aLine);
            for (size_t idx = 0; idx < len; ++idx) {
                buffer.push_back(aLine[idx]);
            }
        });
        return buffer;
    }

    template <typename Iterator>
    auto toStringStream(Iterator begin, Iterator end, std::stringstream &buffer) {
        char aLine[LEN];
        std::for_each(begin, end, [&buffer, &aLine](const auto value) {
            sprintf(aLine, "%g\n", value);
            buffer << aLine;
        });
    }

    template <typename Iterator> auto toMemoryWriter(Iterator begin, Iterator end) {
        fmt::MemoryWriter writer;
        std::for_each(begin, end, [&writer](const auto value) { writer << value << "\n"; });
        return writer;
    }

    // A modified version of the original approach.
    template <typename Container>
    void original_approach(const Container &data, const std::string &fileName) {
        std::ofstream fout(fileName);
        for (size_t l = 0; l < data.size(); l++) {
            fout << data[l] << std::endl;
        }
        fout.close();
    }

    // Replace std::endl by "\n"
    template <typename Iterator>
    void improved_original_approach(Iterator begin, Iterator end, const std::string &fileName) {
        std::ofstream fout(fileName);
        for (Iterator it = begin; it != end; ++it) {
            fout << *it << "\n";
        }
        fout.close();
    }

    // Cache to a string stream before writing to the output file
    template <typename Iterator>
    void stringstream_approach(Iterator begin, Iterator end, const std::string &fileName) {
        std::stringstream buffer;
        for (Iterator it = begin; it != end; ++it) {
            buffer << *it << "\n";
        }

        // Now write to the output file.
        std::ofstream fout(fileName);
        fout << buffer.str();
        fout.close();
    }

    // Use sprintf
    template <typename Iterator>
    void sprintf_approach(Iterator begin, Iterator end, const std::string &fileName) {
        std::stringstream buffer;
        toStringStream(begin, end, buffer);
        std::ofstream fout(fileName);
        fout << buffer.str();
        fout.close();
    }

    // Use fmt::MemoryWriter (https://github.com/fmtlib/fmt)
    template <typename Iterator>
    void fmt_approach(Iterator begin, Iterator end, const std::string &fileName) {
        auto writer = toMemoryWriter(begin, end);
        std::ofstream fout(fileName);
        fout << writer.str();
        fout.close();
    }

    // Use std::vector<char>
    template <typename Iterator>
    void vector_of_char_approach(Iterator begin, Iterator end, const std::string &fileName) {
        std::vector<char> buffer = toVectorOfChar(begin, end);
        std::ofstream fout(fileName);
        fout << buffer.data();
        fout.close();
    }

    // Use cereal (http://uscilab.github.io/cereal/).
    template <typename Container, typename OArchive = cereal::BinaryOutputArchive>
    void use_cereal(Container &&data, const std::string &fileName) {
        std::stringstream buffer;
        {
            OArchive oar(buffer);
            oar(data);
        }

        std::ofstream fout(fileName);
        fout << buffer.str();
        fout.close();
    }
}
