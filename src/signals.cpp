#include "signals.hpp"
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "jobs.hpp"
#include "executor.hpp"


pid_t foreground_pid = -1;
int lastExitStatus = 700;
bool isSignal = false;

void handleSigint(int sig, siginfo_t *info, void *context) {
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT);
            std::cout << "SIGINT has been sent " << std::endl;
            globalJobs.updateJobStatus(-foreground_pid, "Terminated");
        
    } else {
        isSignal = true;
        std::cout << "No foreground process running. Can't Use Ctrl + C" << std::endl;
    }
}

void setupSigintHandler() {
    struct sigaction sa;
    sa.sa_sigaction = handleSigint;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &sa, nullptr);
}


void handleSigtstp(int sig, siginfo_t *info, void *context) {
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGTSTP);
            std::cout << "SIGTSTP has been sent " << std::endl;
            globalJobs.updateJobStatus(foreground_pid, "Stopped");
        
    } else {
        isSignal = true;
        std::cout << "No foreground process running.Can't Use Ctrl + Z" << std::endl;
    }
}

void setupSigtstpHandler() {
    struct sigaction sa;
    sa.sa_sigaction = handleSigtstp;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGTSTP, &sa, nullptr);
}

void setForegroundPid(int pid) {
    foreground_pid = pid;
}

void setLastExitStatus(int status) {
    lastExitStatus = status;
}



void handleSigChld(int) {
    int status;

    auto allJobs = globalJobs.getAllJobs(); 
    for (auto &job : *allJobs) {
        if (! job.backgroundRunning) {
            continue;   // skip any job that’s in foreground
        }

        pid_t r = waitpid(job.pid, &status, WNOHANG);
        if (r <= 0) {
            continue;  
        }
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            globalJobs.updateJobStatus(r, "Done");
        
        }
    }
}




void setupSigChldHandler() {
    struct sigaction sa;
    sa.sa_handler = handleSigChld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;

    sigaction(SIGCHLD, &sa, nullptr);
}
