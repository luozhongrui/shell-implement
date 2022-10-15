#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> mySplit(const std::string &str,
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
