#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
//#include <filesystem>

#include "linux_parser.h"


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



// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
 { 
  string line;
  string word;

  string MemTotal;
  string MemFree;
  string MemAvailable;
  string Buffers;
  float free;
  float total;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
    {
      while(std::getline(stream,line))
      {
      std::stringstream ss(line);
      ss>>word;
      if (word == "MemTotal:")
      {ss>>MemTotal;}
      if (word == "MemAvailable:")
      {
      ss>>MemFree;
      break;
      }
      }
      free = stol(MemFree);
      total = stol(MemTotal);
  
}
return (total-free)/total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
 { 
   string line;
   string word;

    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open())
    {
      std::getline(stream,line);
      std::stringstream ss(line);
      ss>>word;
    }
   return std::stol(word); 
 }


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
 return LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  long t_time;
  string line, value;
  vector<string> s;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream ss(line);
    while (ss >> value) {
    s.push_back(value);
    }
  }
  t_time = (stol(s[13])+stol(s[14])+stol(s[15])+stol(s[16]))/sysconf(_SC_CLK_TCK);

 return t_time; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
 {
  std::vector<std::string>  a = LinuxParser::CpuUtilization();
  long f;
  f = stol(a[CPUStates::kUser_])+stol(a[CPUStates::kSystem_])+
  stol(a[CPUStates::kSteal_])+stol(a[CPUStates::kSoftIRQ_])+
  stol(a[CPUStates::kNice_])+stol(a[CPUStates::kIRQ_]);
  return f;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
std::vector<std::string>  a = LinuxParser::CpuUtilization();
  long f;
  f = stol(a[CPUStates::kIdle_])+stol(a[CPUStates::kIOwait_]);
  return f;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
   string line;
   std::vector<std::string> almacenar;

   string word;
   string user;
   string nice;
   string system;
   string idle;
   string iowait;
   string irq;
   string softirq;
   string steal;
   string guest;
   string guest_nice;

   //int average_time;

    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open())
    {
      std::getline(stream,line);
      std::stringstream ss(line);

      ss >> word;
      ss >> user;
      ss >> nice;
      ss >> system;
      ss >> idle;
      ss >> iowait;
      ss >> irq;
      ss >> softirq;
      ss >> steal;
      ss >> guest;
      ss >> guest_nice;

      almacenar.push_back(user);
      almacenar.push_back(nice);
      almacenar.push_back(system);
      almacenar.push_back(idle);
      almacenar.push_back(iowait);
      almacenar.push_back(irq);
      almacenar.push_back(softirq);
      almacenar.push_back(steal);
      almacenar.push_back(guest);
      almacenar.push_back(guest_nice);
      
    }
   return  almacenar;
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
 {
   string line;
   string word;
   int processes;
 
    std::ifstream stream(kProcDirectory + kStatFilename);
    while (stream.is_open())
    {
      std::getline(stream,line);
      std::stringstream ss(line);
      ss>>word;
      if (word == "processes")
      {
         ss>>word;
         processes = stoi(word);
         break;
      }
    }
    return processes;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
 { 
   string line;
   string word;
   int processes;
 
    std::ifstream stream(kProcDirectory + kStatFilename);
    while (stream.is_open())
    {
      std::getline(stream,line);
      std::stringstream ss(line);
      ss>>word;
      if (word == "procs_running")
      {
         ss>>word;
         processes = stoi(word);
         break;

      }
    }
    return processes;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
   string line;
   string word;
 
    std::ifstream stream(kProcDirectory +"/"+to_string(pid)+ kCmdlineFilename);
    if (stream.is_open())
    {
      std::getline(stream,line);
    }
   return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
   string line;
   string word, word1;
   long number;
 
    std::ifstream stream(kProcDirectory +"/"+to_string(pid)+ kStatusFilename);
    while (stream.is_open())
    {
      std::getline(stream,line);
      std::stringstream ss(line);
      ss>>word;
      if (word == "VmSize:")
      {
         ss>>number;
         number /=1000;
         word1 = to_string(number);
         //ss>>word;
         break;
      }
    }
   return word1;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
   string line;
   string word;
 
    std::ifstream stream(kProcDirectory +"/"+to_string(pid)+ kStatusFilename);
    while (stream.is_open())
    {
      std::getline(stream,line);
      std::stringstream ss(line);
      ss>>word;
      if (word == "Uid:")
      {
         ss>>word;
         break;
      }
    }
   return word;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  // Read and return the user associated with a process
  string uid = Uid(pid);
  string x1, x2, x3, line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream ss(line);
      ss >> x1 >> x2 >> x3;
      if (x3 == uid) {
        break;
      }
    }
  }
  return x1;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
   string line;
   string word;
   int i =0;
 
    std::ifstream stream(kProcDirectory +"/"+to_string(pid)+ kStatFilename);
    while (stream.is_open())
    { i++;
      std::getline(stream,line);
      std::stringstream ss(line);
      ss>>word;
      if (i == 22)
      {
         break;
      }
    }
   return stol(word);
}
