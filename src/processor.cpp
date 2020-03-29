#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float cpuUtil = 0.0;

  std::string line;
  std::string key;

  std::string userS = "", niceS = "", systemS = "", idleS = "", iowaitS = "",
              irqS = "", softirqS = "", stealS = "";

  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> userS >> niceS >> systemS >> idleS >>
             iowaitS >> irqS >> softirqS >> stealS) {
        if (key == "cpu") {
          // // Method 01 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
          // float userF = std::stof(userS);
          // float niceF = std::stof(niceS);
          // float systemF = std::stof(systemS);
          // float idleF = std::stof(idleS);
          // float iowaitF = std::stof(iowaitS);
          // float irqF = std::stof(irqS);
          // float softirqF = std::stof(softirqS);
          // float stealF = std::stof(stealS);

          // float idle = idleF + iowaitF;
          // float nonIdle = userF + niceF + systemF + irqF + softirqF + stealF;
          // float total = idle + nonIdle;
          // float prevTotal = prevIdle_ + prevNonIdle_;

          // float deltaTotal = total - prevTotal;
          // float deltaIdle = idle - prevIdle_;

          // cpuUtil = (deltaTotal - deltaIdle) / deltaTotal;

          // prevIdle_ = idle;
          // prevNonIdle_ = nonIdle;

          // // Method 02 * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *
          std::vector<float> cpuValuesVec(8);
          // cpuValuesVec[0] = std::stof(user);
          cpuValuesVec[LinuxParser::kUser_] = std::stof(userS);
          // This is also right
          // cpuValuesVec[LinuxParser::CPUStates::kUser_] = std::stof(userS);
          cpuValuesVec[LinuxParser::kNice_] = std::stof(niceS);
          cpuValuesVec[LinuxParser::kSystem_] = std::stof(systemS);
          cpuValuesVec[LinuxParser::kIdle_] = std::stof(idleS);
          cpuValuesVec[LinuxParser::kIOwait_] = std::stof(iowaitS);
          cpuValuesVec[LinuxParser::kIRQ_] = std::stof(irqS);
          cpuValuesVec[LinuxParser::kSoftIRQ_] = std::stof(softirqS);
          cpuValuesVec[LinuxParser::kSteal_] = std::stof(stealS);

          float idle = cpuValuesVec[LinuxParser::kIdle_] +
                       cpuValuesVec[LinuxParser::kIOwait_];
          float nonIdle = cpuValuesVec[LinuxParser::kUser_] +
                          cpuValuesVec[LinuxParser::kNice_] +
                          cpuValuesVec[LinuxParser::kSystem_] +
                          cpuValuesVec[LinuxParser::kIRQ_] +
                          cpuValuesVec[LinuxParser::kSoftIRQ_] +
                          cpuValuesVec[LinuxParser::kSteal_];

          float total = idle + nonIdle;
          float prevTotal = prevIdle_ + prevNonIdle_;

          float deltaTotal = total - prevTotal;
          float deltaIdle = idle - prevIdle_;

          cpuUtil = (deltaTotal - deltaIdle) / deltaTotal;

          prevIdle_ = idle;
          prevNonIdle_ = nonIdle;
        }
      }
    }
  }

  return cpuUtil;
}