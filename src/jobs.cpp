#include "jobs.hpp"
#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "executor.hpp"
#include "signals.hpp"


int nextJobId = 1;

Jobs::Jobs() : nextJobId(1) { }


void Jobs::addJob(Job job, pid_t pid) {
    job.pid = pid;
    jobs.push_back(job);
}

void Jobs::updateJobStatus(pid_t pid, const std::string& newStatus) {
    for (auto &job : jobs) {
        if (job.pid == pid) {
            job.status = newStatus;

            if(newStatus == "Done" && job.backgroundRunning) {
                std::cout << pid << "   -Done" << std::endl;
            }
            
            break;
        }
    }
}

void Jobs::listJobs() {
    for (const auto &job : jobs) {
        if (job.status == "Running" || job.status == "Stopped") {
            std::cout << "[" << job.jobId << "]\t" << job.status << "\t";
            for (auto &token : job.command)
                std::cout << token << " ";
            std::cout << std::endl;
        }
        
    }
}

int Jobs::getLastJobId() {
    if (jobs.empty()) return 0;
    return jobs.back().jobId;
}


bool Jobs::continueBackground(int jobID ) {
    for (auto &job: jobs) {
        if(job.jobId == jobID && job.status != "Done") {
            job.status = "Running";
            job.backgroundRunning = true;
             if (kill(job.pid, SIGCONT) < 0) {
                std::cout << "Failed to continue the process:  " << job.pid << std::endl;
             } else {
                std::cout << " Successfully send the SIGCONT signal to the background process " << std::endl;
             }

            
            std::cout << "[" << job.jobId << "]    " << job.pid  << " is continued" << std::endl;
            return true;
        }
    }
    return false;
}

bool Jobs::bringToForeground(int jobID) {
    for (auto &job : jobs) {
        if (job.jobId != jobID || job.status == "Done") {
            continue;
        }

        pid_t pid = job.pid;
        job.backgroundRunning = false;
        job.status = "Running";
        setForegroundPid(pid);

        if (kill(pid, SIGCONT) < 0) {
            perror("kill(SIGCONT)");
            return false;
        }

        int status;
        pid_t p = waitpid(pid, &status, WUNTRACED);

        while (p == -1) {
            p = waitpid(pid, &status, WUNTRACED);
        }
        

        if (p == -1) {  // waitpid itself errored out
            perror("waitpid");
            setLastExitStatus(1);
        } else if (WIFSTOPPED(status)) { // Ctrl-Z again
            job.backgroundRunning = true;
            job.status = "Stopped";
            setLastExitStatus(128 + WSTOPSIG(status));
            std::cout << "\n[" << job.jobId << "] Stopped\t" 
                      << pid << std::endl;
        } else { // either exited normally or was killed by a signal
            job.status = "Done";
            int exitCode;
            if (WIFEXITED(status)) {
                exitCode = WEXITSTATUS(status);
            }
            else {
                exitCode = 128 + WTERMSIG(status);
            }
            setLastExitStatus(exitCode);
        }

        setForegroundPid(-1);
        return true;
    }
    return false;
}


Job* Jobs::getJobById(int jobId) {
    for (auto &job : jobs) {
        if (job.jobId == jobId) {
            return &job;
        }
    }
    return nullptr;
}

Job* Jobs::getJobByPid(pid_t pid) {
    for (auto &job : jobs) {
        if (job.pid == pid) {
            return &job;
        }
    }
    return nullptr;
}


bool Jobs::isThereRunning() {
    for (auto &job: jobs) {
        if (job.status == "Running" && job.backgroundRunning) {
            return true;
        }
    }
    return false;
}
Job* Jobs::getLastJob() {
    if (jobs.empty()) return nullptr;
    return &jobs.back();             
}

std::vector<Job>* Jobs::getAllJobs() {
    return &jobs;
}