#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

void System::Update() {
    memoryUtil_ = LinuxParser::MemoryUtilization();
    upTime_ = LinuxParser::UpTime();
    totalProcs_ = LinuxParser::TotalProcesses();
    procsRunning_ = LinuxParser::RunningProcesses();

    UpdateProcesses();
}

void System::UpdateProcesses(){
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();

    for (auto const& pid : pids){
        Process newProcess(pid);
        processes_.push_back(newProcess);
    }

    std::sort(processes_.begin(), processes_.end());

    
}
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { return processes_; }

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return memoryUtil_; }

std::string System::OperatingSystem() { return os_; }

int System::RunningProcesses() { return procsRunning_; }

int System::TotalProcesses() { return totalProcs_; }

long int System::UpTime() { return upTime_; }