#include <iostream>
#include <string>
#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>
using namespace folly;
using namespace std;

void foo(int x) {
    // do something with x
    cout << "foo(" << x << ")" << endl;
}

int main() {
    // ...
    folly::ThreadedExecutor executor;
    cout << "making Promise" << endl;
    Promise<int> p;
    Future<int> f = p.getSemiFuture().via(&executor);
    auto f2 = f.then(foo);
    cout << "Future chain made" << endl;

    // ... now perhaps in another event callback

    cout << "fulfilling Promise" << endl;
    p.setValue(42);
    f2.get();
    cout << "Promise fulfilled" << endl;
}
