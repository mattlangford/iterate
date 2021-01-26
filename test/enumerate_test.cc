#include "enumerate.hh"

#include <gtest/gtest.h>

#include <vector>

struct TestType {
    size_t i;
    ~TestType() { i = -1; }  // reset I if this gets deconstructed
};

TEST(Enumerate, reference) {
    std::vector<TestType> input{{0}, {1}, {2}};
    for (auto [i, test_type] : it::enumerate(input)) {
        EXPECT_EQ(i, test_type.i);
    }
}
