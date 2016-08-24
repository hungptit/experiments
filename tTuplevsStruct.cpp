#include "StructAndTuple.hpp"

void test_sum() {}

int main() {
    size_t N = 200000000;
    using StructContainer = std::vector<Data>;
    using TupleContainer = std::vector<TupleData>;

    constexpr int LOOP = 5;

    // for (auto idx = 1; idx < LOOP; ++idx) {
    //     StructContainer sdata = test_data<StructContainer>(N);
    //     TupleContainer tdata = create_tuple_data<TupleContainer>(sdata);
    //     test_sort(tdata, "Sort for tuple: ");
    //     test_sort(sdata, "Sort for struct: ");
    // }


    for (auto idx = 1; idx < LOOP; ++idx) {
        StructContainer sdata = test_data<StructContainer>(N);
        TupleContainer tdata = create_tuple_data<TupleContainer>(sdata);
        std::cout << test_sum_tuple(tdata, "Sum for tuple: ") << "\n";
        std::cout << test_sum_struct(sdata, "Sum for struct: ") << "\n";;
    }

    for (auto idx = 1; idx < LOOP; ++idx) {
        StructContainer sdata = test_data<StructContainer>(N);
        TupleContainer tdata = create_tuple_data<TupleContainer>(sdata);
        std::cout << test_sum_struct(sdata, "Sum for struct: ") << "\n";;
        std::cout << test_sum_tuple(tdata, "Sum for tuple: ") << "\n";
    }
}
