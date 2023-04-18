# Build
To build you need to use bazel, which can be installed on osx via homebrew:
`brew install bazel`

http-parser requires C++14 or higher, only because of the googletest framework:
`bazel build  --cxxopt='-std=c++14' //http-parser:http-parser`

to run test:
`bazel test  --cxxopt='-std=c++14'  --test_output=all //:http_parser_unit_tests`

if you dont want to run test, then you can also build with c++11, I have not tested
wwith older c++ standard, but it should work (unless any of the dependencies require a higher version)
