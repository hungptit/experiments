#include "fmt/format.h"
#include "hs/hs.h"
#include "ioutils/ioutils.hpp"
#include <string>

namespace hyperscan {
    class RegexMatcher {
      public:
        RegexMatcher(const std::string &patt) {
            pattern = patt;
            hs_compile_error_t *compile_err;
            if (hs_compile(pattern.c_str(), HS_FLAG_DOTALL, HS_MODE_BLOCK, NULL, &database,
                           &compile_err) != HS_SUCCESS) {
                fmt::MemoryWriter writer;
                writer << "ERROR: Unable to compile pattern \"" << pattern
                       << "\": " << compile_err->message;
                throw std::runtime_error(writer.str());
            }

            if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
                fmt::MemoryWriter writer;
                throw std::runtime_error("Unable to allocate scratch space.");
            }
        }

        bool operator()(const std::string &data) {
            char *ptr = const_cast<char *>(pattern.c_str());
            auto errcode =
                hs_scan(database, data.data(), data.size(), 0, scratch, event_handler, ptr);
            if (errcode == HS_SUCCESS) {
				return false;
            } else if (errcode == HS_SCAN_TERMINATED) {
				return true;
            } else {
                throw std::runtime_error("Unable to scan input buffer");
			}
        }

        ~RegexMatcher() {
            hs_free_scratch(scratch);
            hs_free_database(database);
        }

      private:
        hs_database_t *database = NULL;
        hs_scratch_t *scratch = NULL;
        std::string pattern;
        static unsigned int counter;
        // An event handle callback.
        static int event_handler(unsigned int id, unsigned long long from,
                                 unsigned long long to, unsigned int flags, void *ctx) {
            fmt::print("Match for pattern {0} at offset {1}\n", (char *)ctx, to);
            return HS_SCAN_TERMINATED; // Found a match. Stop our search.
        }
    };
} // namespace hyperscan

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern> <input file>\n", argv[0]);
        return -1;
    }

    char *pattern = argv[1];
    char *afile = argv[2];

    /* Read the input data file into a buffer. */
    const std::string data = ioutils::read<std::string>(afile);
    hyperscan::RegexMatcher matcher(pattern);
    fmt::print("Found: {}\n", matcher(data) ? "Yes" : "No");
    return 0;
}
