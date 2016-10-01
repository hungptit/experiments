#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>

// https://github.com/fmtlib/fmt
#include "fmt/format.h"

// http://uscilab.github.io/cereal/
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

// https://github.com/DigitalInBlue/Celero
#include "celero/Celero.h"

template <typename T> const char* getFormattedString();
template<> const char* getFormattedString<double>(){return "%g\n";}
template<> const char* getFormattedString<float>(){return "%g\n";}
template<> const char* getFormattedString<int>(){return "%d\n";}
template<> const char* getFormattedString<size_t>(){return "%lu\n";}


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
        const char* fmtStr = getFormattedString<typename std::iterator_traits<Iterator>::value_type>();
        std::for_each(begin, end, [&buffer, &aLine, &fmtStr](const auto value) {
            sprintf(aLine, fmtStr, value);
            for (size_t idx = 0; aLine[idx] != 0; ++idx) {
                buffer.push_back(aLine[idx]);
            }
        });
        return buffer;
    }

    template <typename Iterator>
    auto toStringStream(Iterator begin, Iterator end, std::stringstream &buffer) {
        char aLine[LEN];
        const char* fmtStr = getFormattedString<typename std::iterator_traits<Iterator>::value_type>();
        std::for_each(begin, end, [&buffer, &aLine, &fmtStr](const auto value) {            
            sprintf(aLine, fmtStr, value);
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
        const size_t len = std::distance(begin, end) * LEN;
        std::vector<char> buffer(len);
        fout.rdbuf()->pubsetbuf(buffer.data(), len);
        for (Iterator it = begin; it != end; ++it) {
            fout << *it << "\n";
        }
        fout.close();
    }

    //
    template <typename Iterator>
    void edgar_rokyan_solution(Iterator begin, Iterator end, const std::string &fileName) {
        std::ofstream fout(fileName);
        std::copy(begin, end, std::ostream_iterator<double>(fout, "\n"));
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

// Performance test input data.
constexpr int NumberOfSamples = 10;
constexpr int NumberOfIterations = 4;
constexpr int N = 3000000;
const auto double_data = create_test_data<double>(N);
const auto float_data = create_test_data<float>(N);
const auto int_data = create_test_data<int>(N);
const auto size_t_data = create_test_data<size_t>(N);

CELERO_MAIN

BASELINE(DoubleVector, original_approach, NumberOfSamples, NumberOfIterations) {
    const std::string fileName("origsol.txt");
    original_approach(double_data, fileName);
}

BENCHMARK(DoubleVector, improved_original_approach, NumberOfSamples, NumberOfIterations) {
    const std::string fileName("improvedsol.txt");
    improved_original_approach(double_data.cbegin(), double_data.cend(), fileName);
}

BENCHMARK(DoubleVector, edgar_rokyan_solution, NumberOfSamples, NumberOfIterations) {
    const std::string fileName("edgar_rokyan_solution.txt");
    edgar_rokyan_solution(double_data.cbegin(), double_data.end(), fileName);
}

BENCHMARK(DoubleVector, stringstream_approach, NumberOfSamples, NumberOfIterations) {
    const std::string fileName("stringstream.txt");
    stringstream_approach(double_data.cbegin(), double_data.cend(), fileName);
}

BENCHMARK(DoubleVector, sprintf_approach, NumberOfSamples, NumberOfIterations) {
    const std::string fileName("sprintf.txt");
    sprintf_approach(double_data.cbegin(), double_data.cend(), fileName);
}

BENCHMARK(DoubleVector, fmt_approach, NumberOfSamples, NumberOfIterations) {
    const std::string fileName("fmt.txt");
    fmt_approach(double_data.cbegin(), double_data.cend(), fileName);
}

BENCHMARK(DoubleVector, vector_of_char_approach, NumberOfSamples, NumberOfIterations) {
    const std::string fileName("vector_of_char.txt");
    vector_of_char_approach(double_data.cbegin(), double_data.cend(), fileName);
}

BENCHMARK(DoubleVector, use_cereal, NumberOfSamples, NumberOfIterations) {
    const std::string fileName("cereal.bin");
    use_cereal(double_data, fileName);
}

// // Benchmark double vector
// BASELINE(DoubleVectorConversion, toStringStream, NumberOfSamples, NumberOfIterations) {
//     std::stringstream output;
//     toStringStream(double_data.cbegin(), double_data.cend(), output);
// }

// BENCHMARK(DoubleVectorConversion, toMemoryWriter, NumberOfSamples, NumberOfIterations) {
//     celero::DoNotOptimizeAway(toMemoryWriter(double_data.cbegin(), double_data.cend()));
// }

// BENCHMARK(DoubleVectorConversion, toVectorOfChar, NumberOfSamples, NumberOfIterations) {
//     celero::DoNotOptimizeAway(toVectorOfChar(double_data.cbegin(), double_data.cend()));
// }

// // Benchmark float vector
// BASELINE(FloatVectorConversion, toStringStream, NumberOfSamples, NumberOfIterations) {
//     std::stringstream output;
//     toStringStream(float_data.cbegin(), float_data.cend(), output);
// }

// BENCHMARK(FloatVectorConversion, toMemoryWriter, NumberOfSamples, NumberOfIterations) {
//     celero::DoNotOptimizeAway(toMemoryWriter(float_data.cbegin(), float_data.cend()));
// }

// BENCHMARK(FloatVectorConversion, toVectorOfChar, NumberOfSamples, NumberOfIterations) {
//     celero::DoNotOptimizeAway(toVectorOfChar(float_data.cbegin(), float_data.cend()));
// }

// // Benchmark int vector
// BASELINE(int_conversion, toStringStream, NumberOfSamples, NumberOfIterations) {
//     std::stringstream output;
//     toStringStream(int_data.cbegin(), int_data.cend(), output);
// }

// BENCHMARK(int_conversion, toMemoryWriter, NumberOfSamples, NumberOfIterations) {
//     celero::DoNotOptimizeAway(toMemoryWriter(int_data.cbegin(), int_data.cend()));
// }

// BENCHMARK(int_conversion, toVectorOfChar, NumberOfSamples, NumberOfIterations) {
//     celero::DoNotOptimizeAway(toVectorOfChar(int_data.cbegin(), int_data.cend()));
// }

// // Benchmark size_t vector
// BASELINE(size_t_conversion, toStringStream, NumberOfSamples, NumberOfIterations) {
//     std::stringstream output;
//     toStringStream(size_t_data.cbegin(), size_t_data.cend(), output);
// }

// BENCHMARK(size_t_conversion, toMemoryWriter, NumberOfSamples, NumberOfIterations) {
//     celero::DoNotOptimizeAway(toMemoryWriter(size_t_data.cbegin(), size_t_data.cend()));
// }

// BENCHMARK(size_t_conversion, toVectorOfChar, NumberOfSamples, NumberOfIterations) {
//     celero::DoNotOptimizeAway(toVectorOfChar(size_t_data.cbegin(), size_t_data.cend()));
// }
