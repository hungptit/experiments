#ifndef StructVsTuple_hpp_
#define StructVsTuple_hpp_

#include <algorithm>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

#include "tbb/parallel_sort.h"
#include "tbb/tbb.h"

#include "Timer.hpp"

using first_type = char;
using second_type = int;
using third_type = char;
using fourth_type = int;

struct Data {
    Data() : first(), second(), third(), fourth() {}

    template <typename T1, typename T2, typename T3, typename T4>
    Data(T1 &&v1, T2 &&v2, T3 &&v3, T4 &&v4) : first(v1), second(v2), third(v3), fourth(v4) {}

    Data(const Data &rhs) {
        first = rhs.first;
        second = rhs.second;
        third = rhs.third;
        fourth = rhs.fourth;
    }

    template <typename T>
    Data(T &&rhs)
        : first(std::move(rhs.first)), second(std::move(rhs.second)),
          third(std::move(rhs.third)), fourth(std::move(rhs.fourth)) {}

    Data &operator=(const Data &rhs) {
        if (this == &rhs)
            return *this;
        first = rhs.first;
        second = rhs.second;
        third = rhs.third;
        fourth = rhs.fourth;
        return *this;
    }

    first_type first;
    second_type second;
    third_type third;
    fourth_type fourth;
};

bool operator<(const Data &lhs, const Data &rhs) {
    return std::tie(lhs.first, lhs.second, lhs.third, lhs.fourth) <
           std::tie(rhs.first, rhs.second, rhs.third, rhs.fourth);
}

bool operator==(const Data &lhs, const Data &rhs) {
    return (lhs.first == rhs.first) && (lhs.second == rhs.second) && (lhs.third == rhs.third) &&
           (lhs.fourth == rhs.fourth);
}

using TupleData = std::tuple<first_type, second_type, third_type, fourth_type>;

template <typename Container> Container test_data(const size_t N) {
    Container data(N);
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> charDist(
        1, std::numeric_limits<int>::max());
    std::uniform_int_distribution<std::mt19937::result_type> intDist(
        1, std::numeric_limits<int>::max());
    using value_type = typename Container::value_type;
    for (size_t idx = 0; idx < N; ++idx) {
        data[idx] = value_type(intDist(rng), charDist(rng), intDist(rng), charDist(rng));
    }
    return data;
}

template <typename T1, typename T2> T1 create_tuple_data(T2 &&data) {
    const size_t N = data.size();
    T1 results(N);
    for (size_t idx = 0; idx < N; ++idx) {
        auto const item = data[idx];
        results[idx] = std::make_tuple(item.first, item.second, item.third, item.fourth);
    }
    return results;
}

template <typename Container> void test_sort(Container &data, const std::string &msg) {
    utils::ElapsedTime<utils::MILLISECOND> t(msg);
    std::sort(data.begin(), data.end());
}

template <typename Container> size_t test_sum_struct(Container &data, const std::string &msg) {
    utils::ElapsedTime<utils::MILLISECOND> t(msg);
    using value_type = typename Container::value_type;
    size_t sum = 0;
    std::for_each(data.begin(), data.end(), [&sum](const value_type &item) {
        sum += item.first + item.second + item.third + item.fourth;
    });
    return sum;
}

template <typename Container> size_t test_sum_tuple(Container &data, const std::string &msg) {
    utils::ElapsedTime<utils::MILLISECOND> t(msg);
    using value_type = typename Container::value_type;
    size_t sum = 0;
    std::for_each(data.begin(), data.end(), [&sum](const value_type &item) {
        sum += std::get<0>(item) + std::get<1>(item) + std::get<2>(item) + std::get<3>(item);
    });
    return sum;
}

template <typename Container> void test_tbb_sort(Container &data, const std::string &msg) {
    utils::ElapsedTime<utils::MILLISECOND> t(msg);
    tbb::parallel_sort(data.begin(), data.end());
}

#endif
