#include <iostream>

class C {
  public:
    C(int const x) : x(x + 2) {}
    void operator()() const { std::cout << x << '\n'; }

  private:
    int x;
};

int main() { C x(1); }
