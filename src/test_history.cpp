#include <iostream>
#include "history.hpp"

int main() {
    History history;

    history.add("ls -l");
    history.add("cd /home");
    history.add("g++ main.cpp");

    std::string last = history.getLastCommand();

    std::cout << "Last command: " << last << std::endl;

    if (last == "g++ main.cpp") {
        std::cout << "Test passed!\n";
    } else {
        std::cout << "Test failed!\n";
    }

    return 0;
}
