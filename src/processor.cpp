#include "processor.h"

#include "linux_parser.h"

// what a useless class man
float Processor::Utilization() { return LinuxParser::CpuUtilization(); }