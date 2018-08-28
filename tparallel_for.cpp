#include "tbb/tbb.h"
#include <cmath>
#include <vector>

#include "utils/timer.hpp"

template <typename Container> void print(Container &&data, const std::string &msg) {
    if (data.size() > 20) {
        return;
    }
    std::cout << msg << "\n";
    std::for_each(data.begin(), data.end(), [](auto const &item) { std::cout << item << " "; });
    std::cout << "\n";
}

template <typename Container> void test_parallel_for(const int size) {
    Container output;
    Container input(size);

    for (int i = 0; i < size; i++) {
        input[i] = i;
    }
    output.reserve(size);

    
    {
        utils::ElapsedTime<utils::MILLISECOND> timer("Parallel for: ");
        tbb::parallel_for(0, size, 1, [&output, &input](int i) {
            output.push_back(
                sqrt(sin(input[i]) * sin(input[i]) + cos(input[i]) * cos(input[i])));
        });
        print(output, "parallel_for");
    }

    {
        utils::ElapsedTime<utils::MILLISECOND> timer("Simple for loop: ");
        const size_t N = size;
        for (size_t idx = 0; idx < N; ++idx) {
            output.push_back(
                sqrt(sin(input[idx]) * sin(input[idx]) + cos(input[idx]) * cos(input[idx])));
        }
        print(output, "simple for");
    }
}

int main() {
    const int size = 20000000;
    // test_parallel_for<std::vector<double>>(size);
    test_parallel_for<tbb::concurrent_vector<double>>(size);

    return 0;
}
