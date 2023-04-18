#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include "http_headers.hpp"

using namespace std;
const std::string test_resources_path = "http-parser/resources/test/";

TEST(http_headers, ConstructNormally) {

    std::fstream fs;
    std::string resource = test_resources_path +"simple_http_response_headers.txt";

    fs.open(resource);
    if (!fs.is_open()) {
        FAIL() << "unable to open file:" << strerror(errno) << endl;
    }

    boost::leaf::result<std::shared_ptr<http_headers>> result =
        http_headers::from(fs);

    std::shared_ptr<http_headers> headers = result.value();

    const status_line& sl = headers->get_status_line();

    EXPECT_EQ(
            "HTTP/1.0",
            sl.version()
            );
    EXPECT_EQ(
            200,
            sl.code()
            );

    EXPECT_EQ(
            "OK",
            sl.status()
            );


    EXPECT_EQ(
            "public",
            headers->get_value("cache-control").value()
            );

    EXPECT_EQ(
            "0",
            headers->get_value("content-length").value()
            );

    EXPECT_EQ(
            "image/svg+xml",
            headers->get_value("content-type").value()
            );

    EXPECT_EQ(
            "Tue, 22 Jun 2021 22:24:42 GMT",
            headers->get_value("date").value()
            );
}
