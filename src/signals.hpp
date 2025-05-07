#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <csignal>
#include <unistd.h>
#include "executor.hpp"

extern pid_t foreground_pid;
extern int lastExitStatus;
extern bool isSignal;

void handleSigint(int sig, siginfo_t *info, void *context);

void setupSigintHandler();

void handleSigtstp(int sig, siginfo_t *info, void *context);

void setupSigtstpHandler();

void setForegroundPid(int pid);

void setLastExitStatus(int status);

void handleSigChld(int sigchld);

void setupSigChldHandler();



#endif
