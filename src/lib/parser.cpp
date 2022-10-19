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
bool Parser::checkNumPipExist(std::string &cmd, std::vector<std::string> &num) {
  std::regex pattern("[|!][\\d]+");
  std::string::const_iterator iterStart = cmd.begin();
  std::string::const_iterator iterEnd = cmd.end();
  std::smatch result;
  bool flag = false;
  while (std::regex_search(iterStart, iterEnd, result, pattern)) {
    // std::cout << result[0] << std::endl;
    num.push_back(result[0]);
    iterStart = result[0].second;
    flag = true;
  }
  return flag;
}

std::vector<std::string> Parser::pipCmdSplit(std::string &cmd) {
  // split cmd into a vector and get the pip number;
  auto cmdSet = split(cmd, "|");
  return cmdSet;
}

void Parser::parse(std::string &cmd) {
  CreateProcess *creater = new CreateProcess();
  bool flag = false;
  numberCmdCheck(cmd);
  launchPipCmd(flag);
  if (flag)
    return;
  std::vector<std::string> numPipeSet;
  if (checkNumPipExist(cmd, numPipeSet)) {
    numberPipeCmdDeal(cmd, numPipeSet);
    return;
  }
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

void Parser::numberPipeCmdDeal(
    std::string &cmd,
    std::vector<std::string>
        &numPipeSet) { // there are multi number pipe in middle.
  auto saveCmds = findNumPattern(cmd, numPipeSet);
  std::vector<std::pair<int, std::string>> onelineSet;
  for (size_t i = 0; i < numPipeSet.size(); i++) {
    std::string num = numPipeSet[i].substr(1);
    int coldDown = std::stoi(num);
    onelineSet.push_back(
        std::make_pair(coldDown, saveCmds[i] + numPipeSet[i][0]));
  }
  // update the cmd and cold down time;
  oneLineNumDeal(onelineSet);
  numPipeCmds.insert(numPipeCmds.end(), onelineSet.begin(), onelineSet.end());
  // debug
  for (auto a : numPipeCmds) {
    printf("{%d, %s}\n", a.first, a.second.c_str());
  }
}
void Parser::numberCmdCheck(std::string &cmd) {
  std::vector<int> idx;
  for (size_t i = 0; i < numPipeCmds.size(); i++) {
    numPipeCmds[i].first -= 1;
    std::vector<std::string> numSet1, numSet2;
    if (numPipeCmds[i].first == 0 && !checkNumPipExist(cmd, numSet1)) {
      // add cmd into this entry.
      std::string newcmd = numPipeCmds[i].second + " " + cmd;
      launchCmds.push_back(newcmd);
      idx.push_back(i);
    }
    if (numPipeCmds[i].first == 0 && checkNumPipExist(cmd, numSet2)) {
      // add cmd into this entry and update the cold down time.
      cmd = numPipeCmds[i].second + cmd;
      numberPipeCmdDeal(cmd, numSet2);
    }
  }
  int j = 0;
  for (int i : idx) {
    numPipeCmds.erase(numPipeCmds.begin() + i - j);
  }
  idx.clear();
}
void Parser::launchPipCmd(bool &flag) {

  // launch cmd and pop the cmd from vector.
  if (launchCmds.size() > 0)
    flag = true;
  for (size_t i = 0; i < launchCmds.size(); i++) {
    printf("%s\n", launchCmds[i].c_str());
  }
  launchCmds.clear();
}

std::vector<std::string> Parser::findNumPattern(std::string &cmd,
                                                std::vector<std::string> &num) {
  std::vector<std::string> pipCmd;
  int pos = 0;
  int begin = 0;
  for (auto patt : num) {
    pos = cmd.find(patt, pos);
    pipCmd.push_back(cmd.substr(begin, pos - begin));
    pos += patt.size();
    begin = pos;
  }
  return pipCmd;
}
void Parser::oneLineNumDeal(std::vector<std::pair<int, std::string>> &oneline) {
  // update the number mid number.
  int size = oneline.size();
  std::vector<int> pop;
  for (int i = 0; i < size; i++) {
    if (oneline[i].first > size - (i + 1)) {
      oneline[i].first = oneline[i].first - (size - (i + 1));
    } else {
      oneline[i].first = oneline[size - (i + 1)].first;
      pop.push_back(size - (i + 1));
      oneline[i].second += " " + oneline[size - (i + 1)].second;
    }
  }
}