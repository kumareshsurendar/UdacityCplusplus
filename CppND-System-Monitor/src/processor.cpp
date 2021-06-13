#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::stoi;

// DONE: Return the total CPU utilization
float Processor::Utilization() {

  string line;
  string cpu, usr, nice, sys, idle, io_wait, _irq, software_irq, steal, guest, guest_nice;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lnstream(line);
    lnstream >> cpu >> usr >> nice >> sys >> idle >> io_wait >> _irq >> software_irq >> steal >> guest >> guest_nice;
  }
  // Computes CPU utilization - idle + non-idle time
  float tot_time  = stoi(idle) + stoi(io_wait) + stoi(usr) + stoi(nice) + stoi(sys) + stoi(_irq) + stoi(software_irq) + stoi(steal);
  int idletime    = stoi(idle) + stoi(io_wait);
  return (tot_time - idletime)/tot_time;
}