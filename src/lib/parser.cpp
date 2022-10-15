#include "parser.hpp"
#include "env.hpp"

std::vector<std::string> Parser::split(const std::string &str,
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
bool Parser::checkBuildin() {
  auto cmdSet = split(cmd, " ");
  return (bulidInCmd.count(cmdSet[0])) ? true : false;
}
void Parser::parse() {
  if (checkBuildin()) {
    // build_in command
    auto cmdSet = split(cmd, " ");
    if (cmdSet[0] == "printenv")
      printEnv(cmdSet[1]);
    if (cmdSet[0] == "setenv")
      setEnv(cmdSet[1], cmdSet[2]);
    if (cmdSet[0] == "exit")
      exit(0);
  } else {
    // normal command
  }
}