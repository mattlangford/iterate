#include "iterate.hh"

#include <gtest/gtest.h>

#include <vector>

namespace {
struct TestType {
    size_t i;
    ~TestType() { i = -1; }  // reset if this gets deconstructed
};
struct TestType2 {
    size_t j;
    ~TestType2() { j = -1; }  // reset if this gets deconstructed
};

template <typename T>
constexpr bool kIsConstRef = std::is_const_v<std::remove_reference_t<T>>;

std::vector<TestType> test_type_vector() { return {{0}, {1}, {2}}; }
std::vector<TestType2> test_type2_vector() { return {{100}, {101}, {102}}; }
}  // namespace

TEST(Iterate, enumerate_zip) {
    auto input1 = test_type_vector();
    auto input2 = test_type2_vector();
    for (auto [i, test_types] : it::enumerate(it::zip(input1, input2))) {
        auto& [test_type, test_type2] = test_types;
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_FALSE(kIsConstRef<decltype(test_type2)>);
        EXPECT_EQ(test_type.i + 100, test_type2.j);
        EXPECT_EQ(test_type.i, i);
    }
}

TEST(Iterate, zip_enumerate) {
    const auto input = test_type_vector();
    for (auto [e_1, e_2] : it::zip(it::enumerate(input), it::enumerate(input))) {
        auto& [i1, test_type1] = e_1;
        auto& [i2, test_type2] = e_2;

        EXPECT_EQ(i1, i2);
        EXPECT_EQ(test_type1.i, test_type2.i);
    }
}
