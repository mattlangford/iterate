#include "zip.hh"

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

TEST(Zip, vector_reference) {
    auto input1 = test_type_vector();
    auto input2 = test_type2_vector();
    for (auto [test_type, test_type2] : it::zip(input1, input2)) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_FALSE(kIsConstRef<decltype(test_type2)>);
        EXPECT_EQ(test_type.i + 100, test_type2.j);
    }
}

TEST(Zip, vector_const_reference) {
    const auto input1 = test_type_vector();
    const auto input2 = test_type2_vector();
    for (auto [test_type, test_type2] : it::zip(input1, input2)) {
        EXPECT_TRUE(kIsConstRef<decltype(test_type)>);
        EXPECT_TRUE(kIsConstRef<decltype(test_type2)>);
        EXPECT_EQ(test_type.i + 100, test_type2.j);
    }
}

TEST(Zip, vector_rvalue_reference) {
    for (auto [test_type, test_type2] : it::zip(test_type_vector(), test_type2_vector())) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_FALSE(kIsConstRef<decltype(test_type2)>);
        EXPECT_EQ(test_type.i + 100, test_type2.j);
    }
}

TEST(Zip, vector_copy) {
    const auto input1 = test_type_vector();
    const auto input2 = test_type2_vector();
    for (auto [test_type, test_type2] : it::zip_copy(input1, input2)) {
        EXPECT_FALSE(kIsConstRef<decltype(test_type)>);
        EXPECT_FALSE(kIsConstRef<decltype(test_type2)>);

        test_type.i += 100;
        test_type2.j += 100;
    }

    // Make sure the above loop didn't change anything
    for (auto [test_type, test_type2] : it::zip(input1, input2)) {
        EXPECT_TRUE(kIsConstRef<decltype(test_type)>);
        EXPECT_TRUE(kIsConstRef<decltype(test_type2)>);
        EXPECT_EQ(test_type.i + 100, test_type2.j);
    }
}

TEST(Zip, map_reference) {
    const auto input1 = test_type_vector();
    const auto input2 = test_type2_vector();

    std::map<int, TestType> map;
    for (auto [test_type, test_type2] : it::zip(input1, input2)) {
        map[test_type2.j] = test_type;
    }

    for (auto [test_type, test_type2, m] : it::zip(input1, input2, map)) {
        const auto& [key, value] = m;
        EXPECT_EQ(key, test_type2.j);
        EXPECT_EQ(value.i, test_type.i);
    }
}
