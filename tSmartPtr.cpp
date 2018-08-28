#include <memory>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// TODO: Use Catch2

TEST(SmartPtr, Positive) {
    std::unique_ptr<int> p(new int(1));
    EXPECT_TRUE(p != nullptr);

    std::unique_ptr<int> p1(nullptr);
    p1.release();
    EXPECT_TRUE(p.get() != nullptr);

    int x = 3;
    int* a1[2]{nullptr, nullptr};
    int *a2[2]{};
    int *a3[2]{&x, &x};
    
    EXPECT_TRUE(a1[1] == nullptr);
    EXPECT_TRUE(a1[1] == nullptr);
    EXPECT_TRUE(a2[1] == nullptr);
    EXPECT_EQ(a3[0], &x);
    EXPECT_EQ(a3[1], &x);

    int *a4[2]{};
    EXPECT_EQ(a4[0], nullptr);
    EXPECT_EQ(a4[1], nullptr);
}
