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

TEST(Iterate, zip_reverse) {
    const auto input = test_type_vector();
    auto r_input = input;
    std::reverse(r_input.begin(), r_input.end());

    // Since we just reversed, this should put things back in order
    for (auto [t1, i_t2] : it::zip(it::reverse(r_input), it::enumerate(input))) {
        const auto& [i, t2] = i_t2;
        EXPECT_EQ(i, t1.i);
        EXPECT_EQ(t1.i, t2.i);
    }
}

TEST(Iterate, too_much) {
    const auto input = test_type_vector();
    auto r_input = input;
    std::reverse(r_input.begin(), r_input.end());

    for (auto [e_1, e_2] :
         it::zip(it::enumerate(it::reverse(r_input)), it::enumerate(it::zip(input, it::enumerate(input))))) {
        const auto& [i1, in1] = e_1;
        const auto& [i2, in2] = e_2;
        EXPECT_EQ(i1, i2);

        const auto& [in3, i_in4] = in2;
        const auto& [i3, in5] = i_in4;
        EXPECT_EQ(i3, i2);
        EXPECT_EQ(in1.i, in5.i);
    }
}
