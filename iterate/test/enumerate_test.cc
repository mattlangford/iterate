#include "enumerate.hh"

#include <gtest/gtest.h>

#include <map>
#include <vector>

namespace {
struct TestType {
    size_t i;
    ~TestType() { i = -1; }  // reset if this gets deconstructed
};

template <typename T>
constexpr bool kIsConstRef = std::is_const_v<std::remove_reference_t<T>>;

std::vector<TestType> test_type_vector() { return {{0}, {1}, {2}}; }
}  // namespace

TEST(Enumerate, vector_reference) {
    auto input = test_type_vector();
    for (auto [i, test_type] : it::enumerate(input)) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(i, test_type.i);
    }
}

TEST(Enumerate, vector_const_reference) {
    const auto input = test_type_vector();
    for (auto [i, test_type] : it::enumerate(input)) {
        EXPECT_TRUE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(i, test_type.i);
    }
}

TEST(Enumerate, vector_rvalue_reference) {
    for (auto [i, test_type] : it::enumerate(test_type_vector())) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(i, test_type.i);
    }
}

TEST(Enumerate, vector_copy) {
    auto input = test_type_vector();
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

TEST(Enumerate, map_reference) {
    auto input = test_type_vector();
    std::map<int, TestType> map;
    for (auto [i, test_type] : it::enumerate(input)) map[i + 100] = test_type;

    for (auto [i, item] : it::enumerate(map)) {
        EXPECT_FALSE(kIsConstRef<decltype(item)>);

        const auto& [key, value] = item;
        EXPECT_EQ(i, value.i);
        EXPECT_EQ(i + 100, key);
    }
}
