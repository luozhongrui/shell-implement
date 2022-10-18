#include "parser.hpp"
#include "createProcess.hpp"
#include "env.hpp"
#include <regex>

bool Parser::checkBuildin(std::string &cmd) {
  auto cmdSet = split(cmd, " ");
  return (bulidInCmd.count(cmdSet[0])) ? true : false;
}

bool Parser::checkPipExist(std::string &cmd) {
  return (cmd.find("|") != std::string::npos) ? true : false;
}
bool Parser::checkNumPipExist(std::string &cmd) {
  std::regex pattern("[\\|\\!][\\d]+");
  std::string::const_iterator iterStart = cmd.begin();
  std::string::const_iterator iterEnd = cmd.end();
  std::string temp;
  std::smatch result;
  if (std::regex_search(iterStart, iterEnd, result, pattern)) {
    // std::cout << result[0] << std::endl;
    return true;
  }
  return false;
}

std::vector<std::string> Parser::pipCmdSplit(std::string &cmd) {
  // split cmd into a vector and get the pip number;
  auto cmdSet = split(cmd, "|");
  // for (auto cmd : cmdSet) {
  //   std::cout << cmd << std::endl;
  // }
  return cmdSet;
}

void Parser::parse(std::string &cmd) {
  CreateProcess *creater = new CreateProcess();
  if (checkBuildin(cmd)) {
    // build_in command
    auto cmdSet = split(cmd, " ");
    if (cmdSet[0] == "printenv" && cmdSet.size() > 1) {
      printEnv(cmdSet[1]);
    }

    if (cmdSet[0] == "setenv" && cmdSet.size() > 2)
      setEnv(cmdSet[1], cmdSet[2]);
    if (cmdSet[0] == "exit")
      exit(0);
    return;
  }
  if (checkNumPipExist(cmd)) {
    // number pipe
    return;
  }
  if (checkPipExist(cmd)) {
    // origin pipe
    std::vector<std::string> cmdSet = pipCmdSplit(cmd);
    creater->create(cmdSet);
    return;
  } else {
    creater->create(cmd);
  }
  delete creater;
}