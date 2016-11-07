// https://github.com/DigitalInBlue/Celero
#include "celero/Celero.h"

#include "OptimizeString.hpp"

auto const sdata = read("tTuplevsStruct");
std::string sresults(sdata.size(), 0);
const std::vector<char> vdata = convert(sdata);
std::vector<char> vresults(vdata.size());
char *output = new char[sdata.size() + 1];

constexpr int NumberOfSamples = 50;
constexpr int NumberOfIterations = 10;


CELERO_MAIN

BASELINE(remove_ctrl, basic, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl(sdata);
}

BENCHMARK(remove_ctrl, cstyle, NumberOfSamples, NumberOfIterations) {
    remove_ctrl_cstyle(output, &vdata[0], vdata.size());
}

BENCHMARK(remove_ctrl, mutating, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl_mutating(sdata);
}

BENCHMARK(remove_ctrl, reserve, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl_reserve(sdata);
}

BENCHMARK(remove_ctrl, ref_args, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl_ref_args(sdata);
}

BENCHMARK(remove_ctrl, ref_args_it, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl_ref_args_it(sdata);
}

BENCHMARK(remove_ctrl, ref_args_cit, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl_ref_args_cit(sdata);
}

BENCHMARK(remove_ctrl, range_for, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl_range_for(sdata);
}

BENCHMARK(remove_ctrl, for_each, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl_for_each(sdata);
}

BENCHMARK(remove_ctrl, vector, NumberOfSamples, NumberOfIterations) {
    auto results = remove_ctrl_vector(vdata);
}

BENCHMARK(remove_ctrl, vector_revise, NumberOfSamples, NumberOfIterations) {
  remove_ctrl_vector_revise(vdata.cbegin(), vdata.cend(), vresults.begin());
}

BENCHMARK(remove_ctrl, vector_revise1, NumberOfSamples, NumberOfIterations) {
    remove_ctrl_vector_revise1(vdata, vresults);
}
