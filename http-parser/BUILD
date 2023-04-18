load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")


cc_library(
    name = "http-parser",
    srcs = ["main.cpp", "http_headers.cpp","http_header.hpp", "http_headers.hpp"],
    deps = ["@boost//:smart_ptr", "@boost//:algorithm", "@boost//:leaf"]
)

cc_test(
  name = "http_header",
  size = "small",
  srcs = ["http_header_unittest.cpp"],
  deps = ["@com_google_googletest//:gtest_main", "//http-parser:http-parser"]
)

cc_test(
    name = "status_line",
    size = "small",
    srcs = ["status_line_unittest.cpp"],
    deps = ["@com_google_googletest//:gtest_main", "//http-parser:http-parser"]
)

cc_test(
    name = "http_headers",
    size = "small",
    srcs = ["http_headers_unittest.cpp"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "@boost//:filesystem",
        "//http-parser:http-parser"],
    data =
        glob([
        "resources/test/simple_http_response_headers.txt",
        "resources/test/bad_no_status_line_http_response_headers.txt",
    ]),
)
