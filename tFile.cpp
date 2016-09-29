#include "DataSerialization.hpp"

int main() {
    constexpr size_t N = 3000000;
    auto data = create_test_data<float>(N);
    std::cout << "Number of elements: " << N << "\n";
    original_approach(data, "1.txt");
    improved_original_approach(data.cbegin(), data.cend(), "2.txt");
    stringstream_approach(data.cbegin(), data.cend(), "3.txt");
    sprintf_approach(data.cbegin(), data.cend(), "4.txt");
    fmt_approach(data.cbegin(), data.cend(), "5.txt");
    vector_of_char_approach(data.cbegin(), data.cend(), "6.txt");
    use_cereal(data, "7.bin");
}
