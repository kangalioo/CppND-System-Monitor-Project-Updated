#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

Process::Process(int pid) : pid(pid) {}

// Returns this process's ID
int Process::Pid() { return pid; }

// Returns this process's CPU utilization
float Process::CpuUtilization() { return LinuxParser::CpuUtilization(pid); }

// Returns the command that generated this process
std::string Process::Command() { return LinuxParser::Command(pid); }

// Returns this process's memory utilization
int Process::Ram() { return LinuxParser::Ram(pid); }

// Returns the user (name) that generated this process
std::string Process::User() { return LinuxParser::User(pid); }

// Returns the age of this process (in seconds)
long Process::UpTime() { return LinuxParser::UpTime(pid); }