#include "fmt/format.h"
#include <algorithm>
#include <iostream>

class IOStream {
  protected:
    void print(const char *data) { std::cout << data << "\n"; }
};

class FMT {
  protected:
    void print(const char *data) { fmt::print("{}\n", data); }
};

class NoObfuscate {
  protected:
    const char *transform(const char *line, const size_t) { return line; }
};

class Obfuscate {
  protected:
    const char *transform(const char *line, const size_t len) {
        buffer.assign(line, len);
        for (size_t idx = 0; idx < len; ++idx) {
            buffer[idx] += 10;
        }
        return buffer.data();
    }

  private:
    std::string buffer;
};

template <typename OPolicy, typename OType> class DoSomething : private OPolicy, private OType {
  public:
    void doit(const std::string &data) {
        OPolicy::print(OType::transform(data.data(), data.size()));
    }
};

const std::string line("This is a very long line");

template<typename OPolicy, typename EncryptionPolicy>
void print(const std::string &line) {
	DoSomething<OPolicy, EncryptionPolicy> task;
	task.doit(line);	
}

int main() {
	print<IOStream, NoObfuscate>(line);
	print<IOStream, Obfuscate>(line);
	print<FMT, NoObfuscate>(line);
	print<FMT, Obfuscate>(line);

    return EXIT_SUCCESS;
}
