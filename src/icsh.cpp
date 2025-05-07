/* ICCS227: Project 1: icsh
 * Name:
 * StudentID:
 */

 #include "common.hpp"
 #include "parser.hpp"
 #include "executor.hpp"
 #include "history.hpp"
 #include "jobs.hpp"
 #include <fstream>
 #include <signal.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include "signals.hpp"

 Jobs globalJobs;

 int main(int argc, char* argv[]) {
     std::string input;
     std::istream *input_stream = &std::cin;
     std::ifstream file;
     History history;
     setIsBuiltIn(false);
     



     if (argc > 1) {
        file.open(argv[1]);
        if (!file) {
            std::cerr << "Failed to open script file " << argv[1] << std::endl;
            return 1;
        }
        input_stream = &file;
    }


    setupSigintHandler();
    setupSigtstpHandler();
    setupSigChldHandler();

    while(true) {
    
        if (input_stream == &std::cin) {
            std::cout << "icsh $ ";
        }

        if (!std::getline(*input_stream, input)) {
            if (!isSignal) {
                break; 
            } else {
                std::cin.clear();
            }
            
        }

        if (input.empty()) {
            continue;
        }

        if (input == "!!") {
            std::string last = history.getLastCommand();
            if (last.empty())
                continue;
            std::cout << last << std::endl;
            setLastExitStatus(0);
            input = last;
        }

        history.add(input);

        auto tokens = parse_input(input);
        if (tokens.empty()) {
            std::cout << "empty tokens" << std::endl;
            continue;
        }

        if (tokens[0] == "echo") {

            for (size_t i = 1; i < tokens.size(); ++i) {
                if (tokens[i] == "$$") {
                    std::cout << getpid() << " ";
                } else if (tokens[i] == "$?") {
                    std::cout << lastExitStatus << " ";
                } else {
                    std::cout << tokens[i] << " ";
                }
            }
            setLastExitStatus(0);
            std::cout << std::endl;
            continue;
            
        }

        if (tokens[0] == "exit") {
            int exit_code = 0;
            if (tokens.size() > 1) {
                try {
                    exit_code = std::stoi(tokens[1]) & 0xFF; // truncate to 8 bits
                } catch (...) {
                    exit_code = 0;
                }
                exit_code = 0;
            }
            std::cout << "bye" << std::endl;
            return exit_code;
        }

        if (tokens[0] == "bg") {
            if (tokens.size() >= 2 && tokens[1][0] == '%') {
              int jobId = std::stoi(tokens[1].substr(1));
              if (globalJobs.continueBackground(jobId)) {
                std::cout << "bg: called on job ID: " << jobId << std::endl;
                setLastExitStatus(0);
              } else {
                std::cout << "bg: no such job %" << jobId << " or the process already finished"<< std::endl;
                setLastExitStatus(1);
              }
            } else {
              std::cout << "bg: no such job %" << std::endl;
              setLastExitStatus(1);
            }
            continue; 
        }

        if (tokens[0] == "fg") {
            if (tokens.size() >= 2 && tokens[1][0] == '%') {
                int jobId = std::stoi(tokens[1].substr(1));
                if (globalJobs.bringToForeground(jobId)) {
                //   std::cout << "fg: called on job ID: " << jobId << std::endl;
                  setLastExitStatus(0);
                } else {
                  std::cout << "fg: no such job %" << jobId << " or the process already finished " << std::endl;
                  setLastExitStatus(1);
                }
              } else {
                std::cout << "Usage: fg %<jobid>" << std::endl;
                setLastExitStatus(1);
              }
              continue; 
        }

        
          

        if(tokens[0] == "jobs") {
            globalJobs.listJobs();
        }

        // For any other command, assume it is an external command.
        
        int status;

        if( tokens[0] != "bg" && tokens[0] != "fg") {
            status = executeExternalCommand(tokens);
        }
        

        isSignal = false;
        
    }

    std::cout << "There is no error :)" << std::endl;


    return 0;
    

 }