#include "parser.hpp"
#include <sstream>
#include <iostream>

std::vector<std::string> parse_input(const std::string &input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
        // std::cout << token << std::endl;
    }

    return tokens;
}