
#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <vector>
#include <string>

extern bool backgroundRunning;
extern bool isBuiltIn;

int executeExternalCommand(const std::vector<std::string> &tokens);

void setBackgroundRunning(bool state);

void setIsBuiltIn(bool state);

#endif
