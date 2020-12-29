#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::string;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
	string line;
	string key;
	string value;
	std::ifstream filestream("/etc/os-release");
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
	return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
	string dummy, kernel;
	string line;
	std::ifstream stream("/proc/version");
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> dummy >> dummy >> kernel;
	}
	return kernel;
}

// BONUS: Update this to use std::filesystem
std::vector<int> LinuxParser::Pids() {
	std::vector<int> pids;
	DIR* directory = opendir("/proc");
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
	std::ifstream meminfo("/proc/meminfo");
	if (!meminfo) return 0.0;

	std::string dummy;

	long mem_total, mem_free, mem_available;  // in kB
	meminfo >> dummy >> mem_total >> dummy >> dummy >> mem_free >> dummy >> dummy >> mem_available;
	return (float) mem_available / mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
	std::ifstream uptime("/proc/uptime");
	if (!uptime) return 0.0;

	float uptime_seconds;
	uptime >> uptime_seconds;
	return (long) uptime_seconds;
}

float LinuxParser::CpuUtilization() {
	std::ifstream stat("/proc/stat");
	if (!stat) return 0.0;

	// **sigh**
	std::string dummy;
	stat >> dummy;

	long total_jiffies = 0;

	std::vector<long> fields;
	for (int i = 0; i < 10; i++) {
		long field;
		stat >> field;

		total_jiffies += field;
		fields.push_back(field);
	}

	auto idle_jiffies = fields[3] + fields[4];

	return 1 - (float) idle_jiffies / total_jiffies;
}

int GetStatValue(std::string path, std::string target_key_name) {
	std::ifstream stat(path);
	if (!stat) return 0;

	std::string line;
	while (std::getline(stat, line)) {
		std::istringstream line_stream(line);
		std::string key_name;
		line_stream >> key_name;
		if (key_name == target_key_name) {
			int n;
			line_stream >> n;
			return n;
		}
	}
	return 0;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
	return GetStatValue("/proc/stat", "processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
	return GetStatValue("/proc/stat", "procs_running");
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
	std::ifstream cmdline("/proc/" + std::to_string(pid) + "/cmdline");
	std::string result;
	cmdline >> result;
	return result;
}

// Read and return the memory used by a process in kilobytes
int LinuxParser::Ram(int pid) {
	return GetStatValue("/proc/" + std::to_string(pid) + "/status", "VmSize:");
}

// Read and return the user ID associated with a process
int LinuxParser::Uid(int pid) {
	return GetStatValue("/proc/" + std::to_string(pid) + "/status", "Uid:");
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
	std::ifstream passwd("/etc/passwd");
	if (!passwd) return "";

	auto target_uid = Uid(pid);

	std::string line;
	while (std::getline(passwd, line)) {
		auto index1 = line.find(":");
		auto index2 = line.find(":", index1 + 1);
		auto index3 = line.find(":", index2 + 1);

		auto uid = std::stoi(line.substr(index2 + 1, index3 - index2 - 1));
		if (uid == target_uid) {
			return line.substr(0, index1);
		}
	}
	// uid is not in passwd file... weird
	return "";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
	std::ifstream stat("/proc/" + std::to_string(pid) + "/stat");
	if (!stat) return 0;

	std::string token;
	for (int i = 0;; i++) {
		stat >> token;
		if (i == 21) {
			auto startup_time = std::stol(token) / sysconf(_SC_CLK_TCK);
			return UpTime() - startup_time;
		}
	}
}

float LinuxParser::CpuUtilization(int pid) {
	std::ifstream stat("/proc/" + std::to_string(pid) + "/stat");
	if (!stat) return 0.0;

	auto total_process_jiffies = 0;

	std::string token;
	for (int i = 0; i < 17; i++) {
		stat >> token;
		if (i >= 13) {
			total_process_jiffies += std::stol(token);
		}
	}
	
	auto total_process_time = total_process_jiffies / sysconf(_SC_CLK_TCK);

	return (float) total_process_time / UpTime(pid);
}