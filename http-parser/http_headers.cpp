#include <boost/smart_ptr/make_shared.hpp>

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <regex>

#include "http_headers.hpp"

const std::string HEADER_SEPERATOR(":");
const std::string STATUS_LINE_SEPERATOR(" ");
const std::regex HEADER_NAME_PATTERN("^[a-zA-Z0-9-]*$");

// fail safe against certain kinds of bugs where the header parsing can keep on going
const int MAX_HEADERS_PARSE = 100;

boost::leaf::result<status_line> http_headers::parse_status_line(std::istream& file_stream) {
    // status line example:
    // HTTP/1.0 200 OK
    
    std::string raw_status_line;
    std::getline(file_stream, raw_status_line);
    
    if (raw_status_line.size()<1) {
        std::cerr << "unable to parse status line:" << raw_status_line << std::endl;
        return boost::leaf::new_error(http_headers::http_headers_error::unable_to_parse);
    }
    
    std::vector< std::string > status_line_splits;
    
    boost::algorithm::split(
                            status_line_splits,
                            raw_status_line,
                            boost::is_any_of(STATUS_LINE_SEPERATOR),
                            boost::algorithm::token_compress_on );
    
    
    if (status_line_splits.size() != 3) {
        return boost::leaf::new_error(http_headers::http_headers_error::unable_to_parse);
    }
    
    status_line _status_line(
                                  status_line_splits.at(0),
                                  std::stoi(status_line_splits.at(1)),
                                  status_line_splits.at(2)
    );
    
    return _status_line;
}

boost::leaf::result<std::shared_ptr<http_headers>> http_headers::from( std::istream& file_stream) {
    
    boost::leaf::result<status_line> status_line_result = parse_status_line(file_stream);
    if (!status_line_result) {
        return boost::leaf::new_error(status_line_result.error());
    }
    
    boost::leaf::result<std::map<std::string, std::string>> headers_result = parse_headers(file_stream);
    if (!headers_result) {
        return boost::leaf::new_error(headers_result.error());
    }

    return std::make_shared<http_headers>(status_line_result.value(),headers_result.value());
}

boost::leaf::result<std::map<std::string, std::string>> http_headers::parse_headers( std::istream& file_stream) {
    
    std::map<std::string, std::string> map;
    
    do {
        std::string raw_header_line;
        std::getline(file_stream, raw_header_line);
        
        if (raw_header_line.size() > 0) {
            boost::leaf::result<void> header_result = parse_header(raw_header_line, map);
            if (!header_result) {
                std::cerr << "unable to parse header, raw_header_line:" << raw_header_line;
            }
        } else {
            break;
        }
        
    } while (map.size() < MAX_HEADERS_PARSE);
    
    return map;
}

boost::leaf::result<void> http_headers::parse_header(const std::string& raw_header_line,
                                                     std::map<std::string, std::string>& map) {
    
    // splits raw_header_line string using : token as the separator
    
    const std::size_t index = raw_header_line.find_first_of(":");
    
    if (index <= 0) {
        std::cerr << "bad header line: " << raw_header_line;
        return boost::leaf::new_error(http_headers::http_headers_error::bad_header);
    }
    
    std::string header_name = raw_header_line.substr(0, index);
    std::string header_value = raw_header_line.substr(index+1);
    
    boost::trim(header_name);
    boost::trim(header_value);
    
    
    if ( !header_name_valid(header_name) || !header_value_valid(header_value) ) {
        
        std::cerr << "failed to validate header line: " << raw_header_line;
        return boost::leaf::new_error(http_headers::http_headers_error::bad_header);
        }
    
    map.insert(std::make_pair(header_name, header_value));
    
    return {};
}

bool http_headers::header_name_valid(const std::string& header_name) {
    //A header name can contain any letter, digit, and the character “-”
    
    return std::regex_match(header_name, HEADER_NAME_PATTERN);
}

bool http_headers::header_value_valid(const std::string& header_value)  {
    //A header value can contain any visible/printable ASCII character
    return std::all_of(header_value.begin(), header_value.end(), ::isprint);
}

boost::leaf::result<std::string> http_headers::get_value(const std::string& header_name) const {
    try {
        return _headers_map.at(header_name);
    } catch (const std::out_of_range& ex) {
        std::cerr << "queried for non existing key:" << header_name << "," << ex.exception::what();
    }
    return {};
}

const status_line& http_headers::get_status_line() const {
    return _status_line;
}

