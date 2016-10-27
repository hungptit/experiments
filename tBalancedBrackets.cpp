#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

bool is_balanced(const std::string &expression) {
    std::array<char, 3> openBrakets = {{'{', '[', '('}};
    std::array<char, 3> closeBrakets = {{'}', ']', ')'}};
    std::vector<char> stack;

    if (expression.empty()) {
        return true;
    }
    
    for (char ch : expression) {
        for (auto och : openBrakets) {
            if (ch == och) {
                stack.push_back(ch);
                break;
            }
        }

        for (size_t idx = 0; idx < closeBrakets.size(); ++idx) {
            if (closeBrakets[idx] == ch) {
                if (stack.empty()) {
                    return false;
                }
                
                if (openBrakets[idx] == stack.back()) {
                    stack.pop_back();
                    break;
                } else {
                    return false;
                }
            }
        }
    }

    return stack.empty();
}

int main () {
    std::vector<std::string> input{"{aaa[a(])}", "{aaa[a(])}", "{aaa)}", "{aaa[aa(((b)))]}", "}))", ""};
    std::for_each(input.cbegin(), input.cend(), [&input](auto & expression){
        std::cout << (is_balanced(expression) ? "YES" : "NO") << '\n';
    });
}
