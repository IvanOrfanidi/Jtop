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

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpuUsage_; }

string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

bool Process::operator<(Process const& a) const { return this->CpuUtilization() > a.CpuUtilization();}

Process::Process(int pid ){
    pid_ = pid;
    command_ = LinuxParser::Command(pid);
    uptime_ = LinuxParser::UpTime(pid);
    float seconds = LinuxParser::UpTime() - (uptime_);
    cpuUsage_ =  (LinuxParser::ActiveJiffies(pid) / sysconf(_SC_CLK_TCK)) / seconds;
    user_ = LinuxParser::User(pid);

    ram_ = LinuxParser::Ram(pid); 

}