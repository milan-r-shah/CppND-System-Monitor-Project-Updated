#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
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
  string os, kernel, ver;
  string line;

  std::ifstream stream(kProcDirectory + kVersionFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> ver;
  }

  return kernel + " " + ver;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal = 0.0;
  float memFree = 0.0;
  std::string line;
  std::string key;
  std::string value;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memTotal = std::stof(value);
        }
        if (key == "MemFree") {
          memFree = std::stof(value);
          break;
        }
      }
    }
  }

  return (memTotal - memFree) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long int upTime = 0;
  std::string upTimeStr = "";
  std::string line;
  std::string firstValue;
  std::string secondValue;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> firstValue >> secondValue) {
        upTimeStr = firstValue;
      }
    }
  }

  upTime = std::stol(upTimeStr);
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcesses = 0;
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          totalProcesses = std::stoi(value);
          break;
        }
      }
    }
  }

  return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcesses = 0;
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          runningProcesses = std::stoi(value);
          break;
        }
      }
    }
  }

  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string line;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
  }

  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  int ram = 0;
  // float ram = 0.0;
  std::string line;
  std::string key, value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          ram = std::stoi(value) / 1000;
          // ram = std::stof(value) / 1000.0;
        }
      }
    }
  }

  return std::to_string(ram);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::string uid = "";
  std::string line, key, value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          uid = value;
          break;
        }
      }
    }
  }

  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string user = "u";
  std::string uid_of_pid = Uid(pid);
  std::string line;
  std::string first_val, x, sec_val;

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> first_val >> x >> sec_val) {
        if (sec_val == uid_of_pid) {
          user = first_val;
          break;
        }
      }
    }
  }

  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long clockTicks = 0;
  std::string line;
  std::string key, value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), '(', '_');
      std::replace(line.begin(), line.end(), ')', '_');
      std::replace(line.begin(), line.end(), '-', '_');
      std::istringstream linestream(line);
      std::istream_iterator<std::string> beg(linestream), end;
      std::vector<std::string> vec(beg, end);
      clockTicks = std::stol(vec[21]);
    }
  }

  return clockTicks / sysconf(_SC_CLK_TCK);
}

float LinuxParser::ProcessCpuUtil(int pid) {
  float processCpuUtil = 0.0;
  long upTime = UpTime();

  std::string line;
  std::string key, value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  int uTimeClkTks = 0;
  int sTimeClkTks = 0;
  int cuTimeClkTks = 0;
  int csTimeClkTks = 0;
  float startTimeClkTks = 0.0;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), '(', '_');
      std::replace(line.begin(), line.end(), ')', '_');
      std::replace(line.begin(), line.end(), '-', '_');
      std::istringstream linestream(line);
      std::istream_iterator<std::string> beg(linestream), end;
      std::vector<std::string> vec(beg, end);
      uTimeClkTks = std::stoi(vec[13]);
      sTimeClkTks = std::stoi(vec[14]);
      cuTimeClkTks = std::stoi(vec[15]);
      csTimeClkTks = std::stoi(vec[16]);
      startTimeClkTks = std::stof(vec[21]);

      float totalTime =
          (float)(uTimeClkTks + sTimeClkTks + cuTimeClkTks + csTimeClkTks);
      float seconds = upTime - startTimeClkTks / sysconf(_SC_CLK_TCK);
      processCpuUtil = 100 * ((totalTime / sysconf(_SC_CLK_TCK)) / seconds);
    }
  }

  return processCpuUtil;
}