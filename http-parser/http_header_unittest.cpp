#include <gtest/gtest.h>
#include "http_header.hpp"

TEST(http_header, ConstructNormally ) {

    http_header* header = new http_header("header-name", "header-value");
    EXPECT_EQ("header-name", header->name());
    EXPECT_EQ("header-value", header->value());
}

TEST(http_header, ConstructWithEmptyString) {
    http_header* header = new http_header("", "");
    EXPECT_EQ("",  header->name());
    EXPECT_EQ("", header->value());
}
