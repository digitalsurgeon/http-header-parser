//
//  http_header.hpp
//  http-parser
//
//  Created by Ahmad Mushtaq on 4/15/23.
//

#ifndef http_header_h
#define http_header_h

#include <string>

// this class is not used.
class http_header {
protected:
    std::string _name;
    std::string _value;
    
public:
    http_header(const std::string& name, const std::string& value) : _name(name), _value(value) {}
    inline std::string name() { return _name; }
    inline std::string value() { return _value; }
};


#endif /* http_header_h */
    
