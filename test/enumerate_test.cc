#include "enumerate.hh"

#include <gtest/gtest.h>

#include <vector>

struct TestType {
    size_t i;
    ~TestType() { i = -1; }  // reset I if this gets deconstructed
};

template <typename T>
constexpr bool kIsConstRef = std::is_const_v<std::remove_reference_t<T>>;

TEST(Enumerate, vector_reference) {
    std::vector<TestType> input{{0}, {1}, {2}};
    for (auto [i, test_type] : it::enumerate(input)) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(i, test_type.i);
    }
}

TEST(Enumerate, vector_const_reference) {
    const std::vector<TestType> input{{0}, {1}, {2}};
    for (auto [i, test_type] : it::enumerate(input)) {
        EXPECT_TRUE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(i, test_type.i);
    }
}

TEST(Enumerate, vector_rvalue_reference) {
    for (auto [i, test_type] : it::enumerate(std::vector<TestType>{{0}, {1}, {2}})) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(i, test_type.i);
    }
}

TEST(Enumerate, vector_copy) {
    std::vector<TestType> input{{0}, {1}, {2}};

    for (auto [i, test_type] : it::enumerate_copy(input)) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(i, test_type.i);
        test_type.i = 100 + i;
    }

    // Make sure the above loop didn't change anything
    for (auto [i, test_type] : it::enumerate(input)) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(i, test_type.i);
    }
}
