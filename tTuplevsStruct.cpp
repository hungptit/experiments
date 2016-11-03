#include <algorithm>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

// https://github.com/DigitalInBlue/Celero
#include "celero/Celero.h"

using T = double;

struct StructData {
    int X;
    int Y;
    T Cost;
    std::string Label;

    bool operator==(const StructData &rhs) {
        return std::tie(X, Y, Cost, Label) == std::tie(rhs.X, rhs.Y, rhs.Cost, rhs.Label);
    }
    
    void swap(StructData &other) {
        std::swap(X, other.X);
        std::swap(Y, other.Y);
        std::swap(Cost, other.Cost);
        std::swap(Label, other.Label);
    }

    bool operator<(const StructData &rhs) {
        return std::tie(X, Y, Cost, Label) < std::tie(rhs.X, rhs.Y, rhs.Cost, rhs.Label);
    }

    // bool operator<(const StructData &rhs) {
    //     return X < rhs.X || (X == rhs.X && (Y < rhs.Y || (Y == rhs.Y && (Cost < rhs.Cost ||
    //     (Cost == rhs.Cost && Label < rhs.Label)))));
    // }
};

void swap(StructData &v1, StructData &v2) { v1.swap(v2); }

using TupleData = std::tuple<int, int, T, std::string>;

std::vector<StructData> test_struct_data(const size_t N) {
    std::vector<StructData> data(N);
    std::transform(data.begin(), data.end(), data.begin(), [N](auto item) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, N);
        item.X = dis(gen);
        item.Y = dis(gen);
        item.Cost = item.X * item.Y;
        item.Label = std::to_string(item.Cost);
        return item;
    });
    return data;
}

std::vector<TupleData> test_tuple_data(const std::vector<StructData> &input) {
    std::vector<TupleData> data(input.size());
    std::transform(input.cbegin(), input.cend(), data.begin(),
                   [](auto item) { return std::tie(item.X, item.Y, item.Cost, item.Label); });
    return data;
}

constexpr int NumberOfSamples = 10;
constexpr int NumberOfIterations = 5;
constexpr size_t N = 1000000;
auto const sdata = test_struct_data(N);
auto const tdata = test_tuple_data(sdata);

std::ostream &operator<<(std::ostream &os, const StructData &obj) {
    std::cout << "(" << obj.X << "," << obj.Y << "," << obj.Cost << "," << obj.Label << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const TupleData &obj) {
    std::cout << "(" << std::get<0>(obj) << "," << std::get<1>(obj) << "," << std::get<2>(obj)
              << "," << std::get<3>(obj) << ")";
    return os;
}

template <typename Container> void print(const Container &data) {
    std::cout << "==== Data ====\n";
    std::for_each(data.begin(), data.end(),
                  [](auto const &item) { std::cout << item << '\n'; });
}

CELERO_MAIN

BASELINE(Sort, struct, NumberOfSamples, NumberOfIterations) {
    std::vector<StructData> data(sdata.begin(), sdata.end());
    std::sort(data.begin(), data.end());
    // print(data);
}

BENCHMARK(Sort, tuple, NumberOfSamples, NumberOfIterations) {
    std::vector<TupleData> data(tdata.begin(), tdata.end());
    std::sort(data.begin(), data.end());
    // print(data);
}
