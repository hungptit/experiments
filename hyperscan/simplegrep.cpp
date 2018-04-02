#include "hs/hs.h"
#include "ioutils/ioutils.hpp"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {
    int event_handler(unsigned int id, unsigned long long from, unsigned long long to,
                      unsigned int flags, void *ctx) {
        printf("Match for pattern \"%s\" at offset %llu\n", (char *)ctx, to);
        return 0;
    }

} // namespace

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern> <input file>\n", argv[0]);
        return -1;
    }

    char *pattern = argv[1];
    char *afile = argv[2];

    /* First, we attempt to compile the pattern provided on the command line.
     * We assume 'DOTALL' semantics, meaning that the '.' meta-character will
     * match newline characters. The compiler will analyse the given pattern and
     * either return a compiled Hyperscan database, or an error message
     * explaining why the pattern didn't compile.
     */
    hs_database_t *database;
    hs_compile_error_t *compile_err;
    if (hs_compile(pattern, HS_FLAG_DOTALL, HS_MODE_BLOCK, NULL, &database, &compile_err) !=
        HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to compile pattern \"%s\": %s\n", pattern,
                compile_err->message);
        hs_free_compile_error(compile_err);
        return -1;
    }

    /* Next, we read the input data file into a buffer. */
    const std::string data = ioutils::read<std::string>(afile);
    unsigned int length = data.size();

    hs_scratch_t *scratch = NULL;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        hs_free_database(database);
        return -1;
    }

    printf("Scanning %u bytes with Hyperscan\n", length);

    if (hs_scan(database, data.data(), length, 0, scratch, event_handler, pattern) !=
        HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
        hs_free_scratch(scratch);
        hs_free_database(database);
        return -1;
    }

    // Clean up
    hs_free_scratch(scratch);
    hs_free_database(database);
    return 0;
}
