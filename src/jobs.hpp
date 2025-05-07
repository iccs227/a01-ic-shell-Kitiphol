#ifndef JOBS_HPP
#define JOBS_HPP

#include <vector>
#include <string>


struct Job {
    int jobId;
    pid_t pid;
    std::vector<std::string> command;
    std::string status; //"Running", "Stopped", "Done"
    bool backgroundRunning;
};

struct Jobs {
private:
    std::vector<Job> jobs;
    int nextJobId;
public:
    Jobs();
    void addJob(Job job, pid_t pid);
    void updateJobStatus(pid_t pid, const std::string& newStatus);
    void listJobs();
    int getLastJobId();
    bool bringToForeground(int jobId);
    bool continueBackground(int jobId);
    Job* getJobById(int jobId);
    Job* getJobByPid(pid_t pid);
    int getJobId();
    bool isThereRunning();
    Job* getLastJob();
    std::vector<Job>* getAllJobs();
};

extern int nextJobId;
extern Jobs globalJobs;

#endif 
