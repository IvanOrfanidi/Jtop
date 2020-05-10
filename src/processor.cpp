#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long startJiffies = LinuxParser::Jiffies();
    long startActiveJiffies = LinuxParser::ActiveJiffies();

    //sleep for 500ms to measure current cpu utilization
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    long endJiffies = LinuxParser::Jiffies();
    long endActiveJiffies = LinuxParser::ActiveJiffies();

    long deltaJiffies = endJiffies - startJiffies;
    long deltaActiveJiffies = endActiveJiffies - startActiveJiffies;

    float utilization = float(deltaActiveJiffies) / float(deltaJiffies);
    return utilization;

 }