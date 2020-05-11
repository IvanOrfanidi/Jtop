#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
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
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
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

float LinuxParser::MemoryUtilization() {
  string free, total;
  string garbage;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // First line in file corresponds to total memory of system
    linestream >> garbage >> total;
    linestream.clear();
    std::getline(stream, line);
    linestream.str(line);
    // Second line corresponds to free memory
    linestream >> garbage >> free;

    float freeMem = stoi(free);
    float totalMem = stoi(total);
    return freeMem / totalMem;
  }
  return -1;
}

long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return (long)std::stod(uptime);
  }
  return 0;
}

long LinuxParser::Jiffies() {
  auto utilization = CpuUtilization();
  long total = 0;
  // Add up all values besides guest*, since those are already included in the
  // previous columns
  for (int i = CPUStates::kUser_; i <= CPUStates::kSteal_; i++) {
    total += stol(utilization[i]);
  }
  return total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function

vector<string> LinuxParser::PidStats(int pid) {
  vector<string> stats = {};
  string stat;
  string line;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i <= 21; i++) {
      linestream >> stat;
      stats.push_back(stat);
    }
  }
  return stats;
}

long LinuxParser::ActiveJiffies(int pid) {
  auto stats = PidStats(pid);

  long jiffies = 0;
  for (int i = 13; i <= 16; i++) {
    jiffies += std::stol(stats[i]);
  }
  return jiffies;
}

long LinuxParser::ActiveJiffies() { return Jiffies() - IdleJiffies(); }

long LinuxParser::IdleJiffies() {
  auto utilization = CpuUtilization();
  long totalIdleJiffies = stol(utilization[CPUStates::kIOwait_]) +
                          stol(utilization[CPUStates::kIdle_]);
  return totalIdleJiffies;
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> utilization = {};
  string cpu, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // Remove 'cpu' token.
    linestream >> cpu;
    for (int i = 0; i < 10; i++) {
      linestream >> cpu;
      utilization.push_back(cpu);
    }
  }
  return utilization;
}

int LinuxParser::TotalProcesses() { return Pids().size(); }

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  string command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + "/comm");
  if (filestream.is_open()) {
    std::getline(filestream, command);
    return command;
  }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key  >> value;
       if (key == "VmSize:") {
          int  val = std::stoi(value);
          return to_string(val / 1000); }
    }
  }
  return "0";
 }

string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string uid = Uid(pid);

  string line;
  string key, x;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value;
       if (uid == value) { return key; }
    }
  }
  return "behan";
}

long LinuxParser::UpTime(int pid [[maybe_unused]]) {
  auto stats = PidStats(pid);
  return std::stof(stats[21]) / sysconf(_SC_CLK_TCK);
}