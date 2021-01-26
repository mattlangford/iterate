cc_library(
    name = "iterate",
    hdrs = glob(["*.hh"]),
    visibility = ["//visibility:public"],
    deps = [],
)

cc_test(
    name = "iterate_test",
    srcs = glob(["test/*.cc"]),
    deps = [
        ":iterate",
        "@gtest//:gtest",
        "@gtest//:gtest_main"
    ]
)
