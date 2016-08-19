#include "tbb/tbb.h"
#include <cmath>
#include <vector>

#include "utils/Timer.hpp"

int main() {
    const int size = 20000000;
    std::vector<double> output(size, 0);
    std::vector<double> input(size, 0);

    for (int i = 0; i < size; i++) {
        input[i] = i;
    }

    utils::ElapsedTime<utils::MILLISECOND> timer("Time: ");
    tbb::parallel_for(0, size, 1000, [&output, &input](int i) {
        output[i] = sqrt(sin(input[i]) * sin(input[i]) + cos(input[i]) * cos(input[i]));
    });

    return 0;
}
