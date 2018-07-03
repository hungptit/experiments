#include "clara.hpp"
#include "fmt/format.h"
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    int width = 0;
    std::string name;
    bool doIt = false;
    std::string command;
    bool help = false;
    auto cli = clara::Help(help) |
               clara::Opt(width, "width")["-w"]["--width"]("How wide should it be?") |
               clara::Opt(name, "name")["-n"]["--name"]("By what name should I be known") |
               clara::Opt(doIt)["-d"]["--doit"]("Do the thing") |
               clara::Arg(command, "command")("which command to run");

    auto result = cli.parse(clara::Args(argc, argv));
    if (!result) {
        fmt::print(stderr, "Error in command line: {}\n", result.errorMessage());
        exit(1);
    }

    // Print out the help document.
    if (help) {
        std::ostringstream oss;
        oss << cli;
        fmt::print("{}", oss.str());
        exit(EXIT_SUCCESS);
    }

    // Check
    fmt::print("width: {}\n", width);
}
