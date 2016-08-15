#include <iostream>
#include <string>

void print() {
    std::cout << "\n";
}


template<typename T, typename ... Args>
void print(T first, Args ... args) {
    std::cout << first << " ";
    print(args...);
}


struct Constraint1 {
    bool isValid(const std::string &str) {
        return str.find("Hello") != std::string::npos;
    }
};

struct Constraint2 {
    bool isValid(const std::string &str) {
        return str.find("World") != std::string::npos;
    }
};

struct Constraint3 {
    bool isValid(const std::string &str) {
        return str.find("Hello World!") != std::string::npos;
    }
};

bool isValid(const std::string &) {return true;}

template<typename T, typename ... Args>
bool isValid(const std::string &aSentence, T &first, Args ... args) {
    return first.isValid(aSentence) && isValid(aSentence, args...);
}


int main() {
    print();
    print(1, "Hello", "World");

    Constraint1 c1;
    Constraint2 c2;
    Constraint3 c3;
    std::cout << isValid("Foo World", c1) << "\n";
    std::cout << isValid("Foo", c1, c2) << "\n";
    std::cout << isValid("Foo", c1, c2, c3) << "\n";
    std::cout << isValid("Hello World", c1) << "\n";
    std::cout << isValid("World", c1, c2) << "\n";
    std::cout << isValid("Hello World!", c1, c2, c3) << "\n";
}
