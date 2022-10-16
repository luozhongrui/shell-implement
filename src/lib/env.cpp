#include "env.hpp"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
void initEnv() { setenv("PATH", "bin:./", 1); }

void printEnv(std::string env) {
  if (const char *envalue = std::getenv(env.c_str())){
    // std::cout << envalue << std::endl;
    std::string msg = std::string(envalue);
    msg += "\n";
    write(STDOUT_FILENO, msg.c_str(), strlen(msg.c_str()));
    }
}

void setEnv(std::string env, std::string value) {
  setenv(env.c_str(), value.c_str(), 1);
}

bool checkCmdexist(std::string cmd) { return true; }

std::vector<std::string> split(const std::string &str,
                               const std::string &pattern) {
  char *strc = new char[strlen(str.c_str()) + 1];
  strcpy(strc, str.c_str()); // string to C-string
  std::vector<std::string> res;
  char *temp = strtok(strc, pattern.c_str());
  while (temp != NULL) {
    res.push_back(std::string(temp));
    temp = strtok(NULL, pattern.c_str());
  }
  delete[] strc;
  return res;
}
