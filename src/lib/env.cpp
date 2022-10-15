#include "env.hpp"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>

void initEnv() { setenv("PATH", "bin:./", 1); }

void printEnv(std::string env) {
  if (const char *envalue = std::getenv(env.c_str()))
    std::cout << envalue << std::endl;
}

void setEnv(std::string env, std::string value) {
  setenv(env.c_str(), value.c_str(), 1);
}
