#include <iostream>

#include "readline/readline.h"
#include "readline/history.h"

using namespace std;

int main()
{
    char *line;
    while ((line = readline("? ")) != nullptr) {
        cout << "[" << line << "]" << endl;
        if (*line) add_history(line);
        free(line);
    }

    return 0;
}
