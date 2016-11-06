#include <algorithm>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

// https://github.com/DigitalInBlue/Celero
#include "celero/Celero.h"

using T = double;

struct StructData {
  public:
    void swap(StructData &other) {
        std::swap(X, other.X);
        std::swap(Y, other.Y);
        std::swap(Cost, other.Cost);
        std::swap(Label, other.Label);
    }

    void setX(const int val) {X = val;}
    void setY(const int val) {Y = val;}
    void setCost(const T val) {Cost = val;}
    void setLabel(const std::string val) {Label = val;}

    auto convertToTuple() const {return std::tie(X, Y, Cost, Label);}
    
    friend bool operator<(const StructData &first, const StructData &second);
    friend bool operator==(const StructData &first, const StructData &second);
    friend std::ostream &operator<<(std::ostream &os, const StructData &obj);

  private:
    int X;
    int Y;
    T Cost;
    std::string Label;
};

void swap(StructData &v1, StructData &v2) { v1.swap(v2); }

bool operator==(const StructData &first, const StructData &second) {
    return std::tie(first.X, first.Y, first.Cost, first.Label) ==
           std::tie(second.X, second.Y, second.Cost, second.Label);
}

bool operator<(const StructData &first, const StructData &second) {
    return std::tie(first.X, first.Y, first.Cost, first.Label) <
           std::tie(second.X, second.Y, second.Cost, second.Label);
}

// bool operator<(const StructData &rhs) {
//     return X < rhs.X || (X == rhs.X && (Y < rhs.Y || (Y == rhs.Y && (Cost < rhs.Cost ||
//     (Cost == rhs.Cost && Label < rhs.Label)))));
// }

using TupleData = std::tuple<int, int, T, std::string>;

std::vector<StructData> test_struct_data(const size_t N) {
    std::vector<StructData> data(N);
    std::transform(data.begin(), data.end(), data.begin(), [N](auto item) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, N);
        item.setX(dis(gen));
        item.setY(dis(gen));
        item.setCost(dis(gen));
        item.setLabel(std::to_string(dis(gen)));
        return item;
    });
    return data;
}

std::vector<TupleData> test_tuple_data(const std::vector<StructData> &input) {
    std::vector<TupleData> data(input.size());
    std::transform(input.cbegin(), input.cend(), data.begin(),
                   [](auto item) { return item.convertToTuple(); });
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
    std::is_sorted(data.cbegin(), data.cend());
    // print(data);
}

BENCHMARK(Sort, tuple, NumberOfSamples, NumberOfIterations) {
    std::vector<TupleData> data(tdata.begin(), tdata.end());
    std::sort(data.begin(), data.end());
    std::is_sorted(data.cbegin(), data.cend());
    // print(data);
}
