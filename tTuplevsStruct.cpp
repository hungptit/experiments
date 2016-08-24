#include "StructAndTuple.hpp"

size_t N = 200000000;
using StructContainer = std::vector<Data>;
using TupleContainer = std::vector<TupleData>;

bool test_compare(const StructContainer &sdata, const TupleContainer &tdata) {
    size_t r1 = 0;
    for (size_t idx = 0; idx < sdata.size(); ++idx) {
        r1 += sdata[idx].first + sdata[idx].second + sdata[idx].third + sdata[idx].fourth;
    }

    size_t r2 = 0;
    for (size_t idx = 0; idx < tdata.size(); ++idx) {
        r2 += std::get<0>(tdata[idx]) + std::get<1>(tdata[idx]) + std::get<2>(tdata[idx]) +
              std::get<3>(tdata[idx]);
    }

    return r1 == r2;
}

int main() {
    Data d;
    TupleData td;

    std::cout << "Struct: " << sizeof(d) << "\nTuple: " << sizeof(td) << "\n";
    
    constexpr int LOOP = 5;
    for (auto idx = 1; idx < LOOP; ++idx) {
        const StructContainer sdata = test_data<StructContainer>(N);
        const TupleContainer tdata = create_tuple_data<TupleContainer>(sdata);
        test_compare(sdata, tdata);
        std::cout << test_sum_tuple(tdata, "Sum for tuple: ") << "\n";
        std::cout << test_sum_struct(sdata, "Sum for struct: ") << "\n";
        std::cout << test_sum_tuple(tdata, "Sum for tuple: ") << "\n";
        std::cout << test_sum_struct(sdata, "Sum for struct: ") << "\n";
    }
}
