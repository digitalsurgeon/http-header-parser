//
//  main.cpp
//  http-parser
//
//  Created by Ahmad Mushtaq on 4/14/23.
//

#include <iostream>
#include <fstream>
#include "http_headers.hpp"

using namespace std;

int main(int argc, const char * argv[]) {

    std::fstream fstream;
    fstream.open("/Users/ahmad/work/http-header-parser/http-parser/resources/test/bad_no_status_line_http_response_headers.txt");
    if (!fstream.is_open()) {
        std::cout << "unable to open the file, exiting." << std::endl;
        exit(EXIT_FAILURE);
    }


    
    boost::leaf::result<std::shared_ptr<http_headers>> r = boost::leaf::try_handle_some(
                                                                                        [&fstream]() -> boost::leaf::result<std::shared_ptr<http_headers>> {
                                                  return http_headers::from(fstream);
                                              },
                                              
                                              [](http_headers::http_headers_error err) -> boost::leaf::result<std::shared_ptr<http_headers>> {
                                                  return {};
                                              }
                                                                                        );
    

    boost::leaf::result<std::shared_ptr<http_headers>> result = http_headers::from(fstream);
    if (!result) {
        std::cout << "unable to parse the headers. tough luck";
        exit(EXIT_FAILURE);
    }

    const std::shared_ptr<http_headers> instance = result.value();
    const boost::leaf::result<string> cache_control = instance->get_value("cache-control");
    
    assert( cache_control->compare("public") == 0);
    


    fstream.close();

    return 0;
}
