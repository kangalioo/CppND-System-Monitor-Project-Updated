#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

// Returns the system's CPU
Processor& System::Cpu() { return cpu_; }

// Returns a container composed of the system's processes
std::vector<Process> System::Processes() {
  std::vector<Process> processes;
  for (auto pid : LinuxParser::Pids()) {
    processes.push_back(Process(pid));
  }
  return processes;
}

// Returns the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Returns the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Returns the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Returns the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Returns the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Returns the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }