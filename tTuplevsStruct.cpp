#include "StructAndTuple.hpp"

void test_sum() {}

int main() {
    size_t N = 20000000;
    using StructContainer = std::vector<Data>;
    using TupleContainer = std::vector<TupleData>;

    constexpr int LOOP = 5;
    for (auto idx = 1; idx < LOOP; ++idx) {
        StructContainer sdata = test_data<StructContainer>(N);
        TupleContainer tdata = create_tuple_data<TupleContainer>(sdata);
        test_sort(sdata, "Sort for struct: ");
        test_sort(tdata, "Sort for tuple: ");
    }

    // {
    //     StructContainer sdata = test_data<StructContainer>(N);
    //     TupleContainer tdata = create_tuple_data<TupleContainer>(sdata);
    //     test_tbb_sort(sdata, "Sort for struct: ");
    //     test_tbb_sort(tdata, "Sort for tuple: ");
    // }
}
