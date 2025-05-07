#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <string>
#include <vector> 

class History {
    private:
        std::vector<std::string> history;
    public:
        void add(const std::string& command);
        std::string getLastCommand() const;
    };
    

#endif