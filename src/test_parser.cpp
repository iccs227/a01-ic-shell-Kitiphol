#include "parser.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

void test_basic_split() {
    std::string input = "hello world";
    std::vector<std::string> expected;
    expected.push_back("hello");
    expected.push_back("world");
    std::vector<std::string> result = parse_input(input);

    assert(result == expected); 
    std::cout << "test_basic_split passed!\n";
}

void test_empty_string() {
    std::string input = "";
    std::vector<std::string> expected;
    std::vector<std::string> result = parse_input(input);

    assert(result == expected);
    std::cout << "test_empty_string passed!\n";
}

void test_spaces_only() {
    std::string input = "   ";
    std::vector<std::string> expected;
    std::vector<std::string> result = parse_input(input);

    assert(result == expected);
    std::cout << "test_spaces_only passed!\n";
}

int main() {
    test_basic_split();
    test_empty_string();
    test_spaces_only();
    std::cout << "All tests passed!\n";
    return 0;
}
