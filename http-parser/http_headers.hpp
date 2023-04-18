#ifndef http_headers_h
#define http_headers_h

#include <boost/shared_ptr.hpp>
#include <boost/leaf.hpp>
#include <map>

class status_line {
    
private:
    std::string _version;
    int         _code;
    std::string _status;
    
public:
    status_line(const std::string& version, int code, const std::string& status) : _version(version), _code(code), _status(status) {}
    
    std::string version() const {return _version;}
    int code() const { return _code; }
    std::string status() const { return _status; }
    
};

class http_headers {
    
private:
    status_line _status_line;
    std::map<std::string, std::string> _headers_map;

public:
    http_headers( status_line& status_line, std::map<std::string, std::string>& headers_map) :
    _status_line(std::move(status_line)), _headers_map(std::move(headers_map)) {}
    
    static boost::leaf::result<std::shared_ptr<http_headers>> from( std::istream& file_stream);
    
    
    boost::leaf::result<std::string> get_value(const std::string& header_name) const;
    const status_line& get_status_line() const;

    enum class http_headers_error { invalid_arguments, unable_to_parse, bad_header };
    
protected:
    
    static boost::leaf::result<status_line> parse_status_line (std::istream& file_stream);
    static boost::leaf::result<std::map<std::string, std::string>> parse_headers (std::istream& file_stream);
    static boost::leaf::result<void>  parse_header (const std::string& raw_header_line, std::map<std::string, std::string>&);
    
    
    static bool header_name_valid(const std::string&);
    static bool header_value_valid(const std::string&);
    
    
};

#endif /* http_headers_h */
