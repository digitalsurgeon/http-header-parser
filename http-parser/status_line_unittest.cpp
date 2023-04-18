#include <gtest/gtest.h>
#include "http_headers.hpp"

TEST(status_line, ConstructNormally) {
    status_line status("http/1.1", 418, "I'm a teapot");
    EXPECT_EQ("http/1.1", status.version());
    EXPECT_EQ(418, status.code());
    EXPECT_EQ("I'm a teapot", status.status());
}

