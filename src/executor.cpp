#include "executor.hpp"
#include "signals.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include "jobs.hpp"


bool backgroundRunning = false;
bool isBuiltIn = false;

void setBackgroundRunning(bool state) {
    backgroundRunning = state;
}

void setIsBuiltIn(bool state) {
    isBuiltIn = state;
}

int executeExternalCommand(const std::vector<std::string> &tokens) {

    if(tokens.empty()) {
        return 0;
    }




    int in = -1;
    int out = -1;
    std::vector<std::string> cleanTokens;
    backgroundRunning = false;
    

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "<") {
            if (i + 1 < tokens.size()) {
                in = open(tokens[i + 1].c_str(), O_RDONLY);
                if (in < 0) {
                    std::perror("Error opening input file");
                    return -1;
                }
                ++i; 
            }
        } else if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) {
                out = open(tokens[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (out < 0) {
                    std::perror("Error opening output file");
                    return -1;
                }
                ++i; 
            }
        } else {
            cleanTokens.push_back(tokens[i]);
        }
    }




   std::vector<char*> args;
   //using const is faster than using std::string in terms of converting to char* (from benchmarking)
   for (const auto &token: cleanTokens) {
    //exec() functions expect char*, not std::string 
    // --> use .c_str() to convert fron const std::string to const char*
    // --> use const_cast to convert from const char* to char*
    if(token == "&") {
        setBackgroundRunning(true);
    } else {
        args.push_back(const_cast<char*>(token.c_str()));
    }
   
   }

   Job j;
   j.command = cleanTokens;
   j.jobId = nextJobId++;
   j.backgroundRunning = backgroundRunning;
   if(j.backgroundRunning) {
    setForegroundPid(-1);
   }
   j.status = "Running";

   // exec() functions expects a null pointer at the end of array input
   args.push_back(nullptr);

   pid_t pid = fork();

   if (pid < 0) {
        std::cerr << "Fork failed." << std::endl;
        return -1;
   } else if (pid == 0) {

        setpgid(0, 0);

        // globalJobs.addJob(j, pid);

        if (in != -1) {
            dup2(in, 0);
            close(in);
        }

        if (out != -1) {
            dup2(out, 1);
            close(out);
        }
        int val = execvp(args[0], args.data());
        if (val == -1) {
            std::cerr << "Failed to execute command: " 
                    << strerror(errno) << std::endl;
            exit(1); //exit(0) for success, exit(1) for failure
        } 

   } else {
        // setpgid(pid, pid);

        if(j.backgroundRunning) { //background
            setpgid(pid, pid);
            setForegroundPid(-1);
            globalJobs.addJob(j, pid);
            std::cout << "[" << globalJobs.getLastJobId() << "] " << pid << std::endl;
            
            setLastExitStatus(0);
            return 0;

        } else { //foreground

            // setpgid(pid, pid);
            setForegroundPid(pid);
            globalJobs.addJob(j, pid);
            int status = 0;
            int waitres = waitpid(pid, &status, WUNTRACED);

            if ( waitres == -1 ) {
                while( (waitres = waitpid(pid, &status, WUNTRACED)) == -1) {
                
                }
            }
            
            
            // globalJobs.addJob(pid, cleanTokens, backgroundRunning);
            globalJobs.updateJobStatus(pid, "Done");
            // std::cout << "This is the status" << std::endl;
            // std::cout << status << std::endl;

            // std::cout << "waitres: " << waitres << std::endl;
            
            

            // std::cout << "Start " << std::endl;
            if (WIFEXITED(status)) {
                // std::cout << "Case A " << std::endl;
                // std::cout << WEXITSTATUS(status) << std::endl;
                setLastExitStatus(WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                // std::cout << "Case B " << std::endl;
                // std::cout << WIFSIGNALED(status) << std::endl;
                // int sig = WTERMSIG(status);
                // std::cout << "Signal: " << sig << "(" << strsignal(sig) << ")" << std::endl;
                // if (errno == EINTR) {
                    
                //     std::cout << "ah" << std::endl;
                // } 
                setLastExitStatus(128 + WTERMSIG(status)); 
            } else if (WIFSTOPPED(status) ){ 
                // std::cout << "Process Stopped" << std::endl;
                globalJobs.updateJobStatus(foreground_pid, "Stopped");
                // std::cout << "Signal: " << WSTOPSIG(status) << "(" << strsignal(WSTOPSIG(status)) << ")" << std::endl;
                setLastExitStatus(128 + WSTOPSIG(status)); 
            } else {
                // std::cout << "Case C " << std::endl;
                setLastExitStatus(1);
            }

            setForegroundPid(-1);
            
            // std::cout << " This is the exist status code " << std::endl;
            // std::cout << lastExitStatus <<std::endl;

            return lastExitStatus;
        }
        
   }

   return 0;

}