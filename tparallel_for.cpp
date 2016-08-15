#include "tbb/tbb.h"
#include <cmath>
#include <vector>

int main() {
    const int size = 20000000;
    std::vector<double> output(size, 0), input(size, 0);

    for (int i = 0; i < size; i++) {
        input[i] = i;
    }

    tbb::parallel_for(0, size, 1, [=](int i) {
        output[i] = sqrt(sin(input[i]) * sin(input[i]) + cos(input[i]) * cos(input[i]));
    });

    return 0;
}
