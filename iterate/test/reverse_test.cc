#include "reverse.hh"

#include <gtest/gtest.h>

#include <vector>

#include "enumerate.hh"

namespace {
struct TestType {
    size_t i;
    ~TestType() { i = -1; }  // reset if this gets deconstructed
};

template <typename T>
constexpr bool kIsConstRef = std::is_const_v<std::remove_reference_t<T>>;

std::vector<TestType> test_type_vector() { return {{0}, {1}, {2}}; }
}  // namespace

TEST(Reverse, vector_reference) {
    auto input = test_type_vector();
    for (auto [i, test_type] : it::enumerate(it::reverse(input))) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(input.size() - i - 1, test_type.i);
    }
}

TEST(Reverse, vector_const_reference) {
    const auto input = test_type_vector();
    for (auto [i, test_type] : it::enumerate(it::reverse(input))) {
        EXPECT_TRUE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(input.size() - i - 1, test_type.i);
    }
}

TEST(Reverse, vector_rvalue_reference) {
    for (auto [i, test_type] : it::enumerate(it::reverse(test_type_vector()))) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_EQ(test_type_vector().size() - i - 1, test_type.i);
    }
}

TEST(Reverse, vector_copy) {
    auto input = test_type_vector();
    std::vector<TestType> output;
    for (auto test_type : it::reverse(input)) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        output.push_back(std::move(test_type));
    }
    ASSERT_EQ(output.size(), input.size());
    EXPECT_EQ(input.front().i, output.back().i);
}
