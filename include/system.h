#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
 public:
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp
  void Update();

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string os_ = LinuxParser::OperatingSystem();
  std::string kernel_ = LinuxParser::Kernel();
  float memoryUtil_ = LinuxParser::MemoryUtilization(); 
  long upTime_ = LinuxParser::UpTime();
  int totalProcs_ = LinuxParser::TotalProcesses();
  int procsRunning_ = LinuxParser::RunningProcesses();
};

#endif