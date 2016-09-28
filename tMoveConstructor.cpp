#include <iostream>

class Base {
  public:
    explicit Base() : data() { std::cout << "Default constructor\n"; };

    explicit Base(std::string &&s) : data(std::move(s)) {
        std::cout << "Default constructor which move string data\n";
    };

    explicit Base(const Base &obj) {
        std::cout << "Copy const ref\n";
        data = obj.data;
    }

    explicit Base(Base &obj) {
        std::cout << "Copy ref\n";
        data = obj.data;
    }

    explicit Base(Base &&) { std::cout << "Move normal\n"; }

    template <typename T> explicit Base(T &&s) : data(std::forward<T>(s)) { std::cout << "Templatized move constructor\n"; }

    void operator()(const std::string &msg) const { std::cout << msg << " and data = " << data << "\n";}

  private:
    std::string data;
};

int main() {
    std::string s("foo");
    
    Base b;
    b("Case 1");

    Base b1("Hello");
    b1("Case 2");

    Base b2(s);
    b2("Case 3");

    Base b3(std::move(s));
    b3("Case 4");

    const Base c(b1);
    c("Case 5");

    const Base &e(c);
    e("Case 6");
    
    const Base &d(e);
    d("Case 7");
    
    Base x(e);
    Base y(c);
    
    {
        // Base x(std::move(e));
        // Base y(std::move(d));
        // Base z(std::move(c));
    }
}
