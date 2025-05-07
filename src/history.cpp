#include "history.hpp"

void History::add(const std::string& command) {
    history.push_back(command);
}

std::string History::getLastCommand() const {
    if (history.empty())
        return "";
    return history.back();
}
