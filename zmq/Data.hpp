#pragma once

#include <vector>
#include <string>

#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/chrono.hpp"
#include "cereal/types/deque.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/tuple.hpp"

#include "fmt/format.h"

using BasicRequest = std::tuple<std::string, std::vector<std::string>>;
using BasicReply = std::tuple<std::string, std::vector<std::string>>; 

using IArchive = cereal::JSONInputArchive;
using OArchive = cereal::JSONOutputArchive;

template <typename T> T decode(std::string &&msg);

enum CommandCode{MLOCATE, MDIFF, MCOPYDIFF, MUPDATEDB, SOURCE2TEST, RUNTEST};

using Message = std::tuple<CommandCode, std::string>;

using Source2TestRequest = std::tuple<CommandCode, std::string, std::vector<std::string>>;
using Source2TestReply = std::tuple<CommandCode, std::vector<std::string>>;


template<>
BasicRequest decode<BasicRequest>(std::string &&msg) {
    BasicRequest data;
    std::istringstream is(msg);
    {
        IArchive input(is);
        input(data);
    }
    return data;
}

// template<>
// BasicReply decode<BasicReply>(std::string &&msg) {
//     BasicRequest data;
//     std::istringstream is(msg);
//     {
//         IArchive input(is);
//         input(data);
//     }
//     return data;
// }


template <typename T> void print(T &&data);

template<> void print<BasicRequest>(BasicRequest &&data) {
    fmt::print("Source node: {}\n", std::get<0>(data));
    fmt::print("Number of folders: {}\n", std::get<1>(data).size());
}

// template<> void print<BasicReply>(BasicReply &&data) {
//     fmt::print("Source node: {}\n", std::get<0>(data));
//     fmt::print("Number of folders: {}\n", std::get<1>(data).size());
// }


