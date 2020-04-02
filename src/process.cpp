#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

int Process::GetPid() const {
    int tempPid = pid_;
    return tempPid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  // return LinuxParser::ProcessCpuUtil(Pid());
  float processCpuUtil = 0.0;
  long upTime = LinuxParser::UpTime();

  std::string line;
  std::string key, value;
  const int tempPid = GetPid();
  std::string tempString = std::to_string(tempPid);

//   std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(tempPid) +
//                            LinuxParser::kStatFilename);
  std::ifstream filestream(LinuxParser::kProcDirectory + tempString +
                           LinuxParser::kStatFilename);

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

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return (this->CpuUtilization() < a.CpuUtilization());
}