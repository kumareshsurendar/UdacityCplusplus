#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return "-1";
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memkey, memtotal, memfree, kb;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    // Get Total Memory
    std::getline(stream, line);
    std::istringstream lnstream(line);
    lnstream >> memkey >> memtotal >> kb;

    // Get Free Memory
    std::getline(stream, line);
    lnstream.str(line);
    lnstream.clear();
    lnstream >> memkey >> memfree >> kb;
  }

  float memused = stof(memtotal) - stof(memfree);
  // Percentage used = (total - free) / total
  float pmemused = memused / stof(memtotal);
  return pmemused;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lnstream(line);
    lnstream >> uptime >> idletime;
  }
  return stol(uptime);
}

// DONE: Read and return the number of jiffies for the system
// INPUT: Process ID
// OUTPUT: Returns the timing parametes of pid in Hz
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string var;
  string utime, stime, cutime, cstime; 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream lntream(line);
      for(int i = 1; i < 14; i++) {
        lntream >> var; 
      }
      lntream >> utime >> stime >> cutime >> cstime; 
      long totaltime = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
      return totaltime;
    }
  }
return -1;
}

// DONE: Returns the tot # of processes
int LinuxParser::TotalProcesses() {
  string key, val, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream lntream(line);
      while (lntream >> key >> val) {
        if (key == "processes") {
          return stoi(val);
        }
      }
    }
  }
  return -1;
}

// DONE: Returns the # of running processes
int LinuxParser::RunningProcesses() {
  string key, val, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream lntream(line);
      while (lntream >> key >> val) {
        if (key == "procs_running") {
          return stoi(val);
        }
      }
    }
  }
  return -1;
}

// DONE: Returns the command associated with a process
string LinuxParser::Command(int pid) {
  string ln;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, ln);
    return ln;
  }
  return "-1";
}

// DONE: Returns the memory used by a process
string LinuxParser::Ram(int pid) {
  string key, val, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream lntream(line);
      while (lntream >> key >> val) {
        if (key == "VmSize:") 
          return to_string(stoi(val) / 1024);
      }
    }
  }
  return "-1";
}

// DONE: return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key, val, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream lntream(line);
      while (lntream >> key >> val) {
        if (key == "Uid:") 
          return val;
      }
    }
  }
  return "-1";
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, usr, a, lhs;
  string rhs{LinuxParser::Uid(pid)};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lnstream(line);
      while (lnstream >> usr >> a >> lhs) {
        if (lhs == rhs) 
          return usr;
      }
    }
  }
  return "-1";
}

// DONE: Return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, var;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream lnstream(line);
      for(int i = 1; i < 23; i++) {
        lnstream >> var; 
      }
      return stol(var) / sysconf(_SC_CLK_TCK); // stackkoverflo 
    }
  }
return -1;
}