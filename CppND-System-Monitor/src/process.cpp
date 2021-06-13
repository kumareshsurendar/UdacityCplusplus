#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Define custom constructor
Process::Process(int pid)
    : pid_(pid),
      command_(LinuxParser::Command(pid)),
      user_(LinuxParser::User(pid)){};

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return process's CPU utilization
float Process::CpuUtilization() {
  float tot_time      = LinuxParser::ActiveJiffies(Pid());  
  float uptime        = LinuxParser::UpTime();                 
  float active_dur    = uptime - (Process::UpTime() / sysconf(_SC_CLK_TCK));  
  float cpu_util      = (tot_time / sysconf(_SC_CLK_TCK)) / active_dur; 
  return cpu_util;
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

 
