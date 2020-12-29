#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();
float CpuUtilization();

// Processes
std::string Command(int pid);
int Ram(int pid);
int Uid(int pid);
std::string User(int pid);
long UpTime(int pid);
float CpuUtilization(int pid);
};  // namespace LinuxParser

#endif