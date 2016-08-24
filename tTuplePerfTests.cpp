#include "StructAndTuple.hpp"

void test_sum() {}

int main() {
    size_t N = 30000000;
    using StructContainer = std::vector<Data>;
    using TupleContainer = std::vector<TupleData>;

    {
        StructContainer sdata = test_data<StructContainer>(N);
        TupleContainer tdata = create_tuple_data<TupleContainer>(sdata);
        test_sort(tdata, "Sort for tuple: ");
    }

    {
        StructContainer sdata = test_data<StructContainer>(N);
        TupleContainer tdata = create_tuple_data<TupleContainer>(sdata);
        test_sort(tdata, "Sort for tuple: ");
    }
}
