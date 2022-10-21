#include "parser.hpp"
#include "env.hpp"
#include <regex>

bool Parser::checkBuildin(std::string &cmd) {
  auto cmdSet = split(cmd, " ");
  return (bulidInCmd.count(cmdSet[0])) ? true : false;
}

bool Parser::checkPipExist(std::string &cmd) {
  return (cmd.find("|") != std::string::npos ||
          cmd.find("!") != std::string::npos)
             ? true
             : false;
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
  bool launch = false;
  bool isNumCmd = false;
  checkStorge(cmd, isNumCmd);
  launchNumCmd(launch, creater);
  if (launch) {
    return;
  }
  std ::vector<std::string> splits;
  if (checkNumPipExist(cmd, splits)) {
    if (!isNumCmd) {
      parseNumPipeCmd(cmd);
      launchNumCmd(launch, creater);
    }
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
    // std::vector<std::string> cmdSet = pipCmdSplit(cmd);
    creater->createPipe(cmd);
    return;
  } else {
    creater->create(cmd);
  }
  delete creater;
}

void Parser::launchNumCmd(bool &launched, CreateProcess *creater) {

  // launch cmd and pop the cmd from vector.
  if (launchCmds.size() > 0) {
    launched = true;
  }
  if (launchCmds.size() > 1) {
    for (size_t i = 0; i < launchCmds.size(); i++) {
      printf("[launched:]%s\n", launchCmds[i].c_str());
    }
  }
  if (launchCmds.size() == 1) {
    creater->createPipe(launchCmds[0]);
  }

  launchCmds.clear();
}

void Parser::parseNumPipeCmd(std::string &cmd) {
  // save cold down entry to storage.
  std::vector<std::vector<std::string>> subPipCmds;
  std::vector<std::vector<std::string>> decol;
  std::vector<std::string> pipCmds;     // whole cmd
  std::vector<std::string> splitPoints; // cold down
  checkNumPipExist(cmd, splitPoints);
  pipCmds = splitNumPoint(cmd, splitPoints);

  for (auto subcmd : pipCmds) {
    std::vector<std::string> deco = *(new std::vector<std::string>);
    auto tmep = splitPipeAndError(subcmd, deco);
    subPipCmds.push_back(tmep);
    decol.push_back(deco);
  }
  // build the entry
  std::vector<numPipEntry *> oneLineCmd;
  for (size_t i = 0; i < splitPoints.size(); i++) {
    std::string splitDesp = "";
    int cold = -1;
    splitDesp = splitPoints[i][0];
    cold = std::stoi(splitPoints[i].substr(1));
    // int weight = subPipCmds[i].size();
    struct numPipEntry *entry =
        new numPipEntry(cold, pipCmds[i], splitDesp, true);
    oneLineCmd.push_back(entry);
    // printf("[%d, %d, %s, %s]", weight, cold, pipCmds[i].c_str(),
    //        splitDesp.c_str());
  }
  // update entry
  for (size_t i = 0; i < oneLineCmd.size(); i++) {
    for (size_t j = i + 1; j < subPipCmds.size(); j++) {
      if (oneLineCmd[i]->cold > subPipCmds[j].size()) {
        oneLineCmd[i]->cold -= subPipCmds[j].size();
      } else {
        int offset = subPipCmds[j].size() - oneLineCmd[i]->cold;
        oneLineCmd[i]->cmd += " " + oneLineCmd[i]->typePipe;
        for (size_t k = subPipCmds[j].size() - offset - 1;
             k < subPipCmds[j].size(); k++) {
          if (k >= decol[j].size()) {
            oneLineCmd[i]->cmd += " " + subPipCmds[j][k] + " |";
          } else {
            oneLineCmd[i]->cmd +=
                " " + subPipCmds[j][k] + decol[j][(k > 0) ? k - 1 : k];
          }
        }
        oneLineCmd[i]->cmd =
            oneLineCmd[i]->cmd.substr(0, oneLineCmd[i]->cmd.size() - 2);

        if (j >= oneLineCmd.size()) {
          // printf("[launc]%s\n", oneLineCmd[i]->cmd.c_str());
          launchCmds.push_back(oneLineCmd[i]->cmd);
          oneLineCmd[i]->flag = false;
          // delete oneLineCmd[i];
        } else {
          if (offset == subPipCmds[j].size() || offset == 0) {
            oneLineCmd[j]->cmd = oneLineCmd[i]->cmd;
            oneLineCmd[i]->flag = false;
          }

          // printf("%s,  %d", oneLineCmd[i]->cmd.c_str(), oneLineCmd[i]->cold);
        }
        break;
      }
    }
  }
  // save to parser
  for (size_t i = 0; i < oneLineCmd.size(); i++) {
    if (oneLineCmd[i]->flag) {
      storge.push_back(oneLineCmd[i]);
    }
  }
}

std::vector<std::string> Parser::splitNumPoint(std::string &cmd,
                                               std::vector<std::string> &num) {
  std::vector<std::string> pipCmd;
  int pos = 0;
  int begin = 0;
  for (auto patt : num) {
    pos = cmd.find(patt, pos);
    pipCmd.push_back(cmd.substr(begin, pos - begin));
    // printf("[%s]\n", cmd.substr(begin, pos - begin).c_str());
    pos += patt.size();
    begin = pos;
  }
  if (begin < cmd.size() - 1) {
    pipCmd.push_back(cmd.substr(begin, cmd.size() - begin));
    // printf("[%s]\n", cmd.substr(begin, cmd.size() - begin).c_str());
  }
  return pipCmd;
}

void Parser::checkStorge(std::string cmd, bool &isNumCmd) {
  int size = storge.size();
  for (int i = 0; i < size; i++) {
    if (storge[i]->flag) {
      storge[i]->cold -= 1;
    }
    if (storge[i]->cold == 0 && storge[i]->flag) {
      storge[i]->flag = false;
      std::string newcmd =
          storge[i]->cmd + " " + storge[i]->typePipe + " " + cmd;
      std::vector<std::string> spliter;
      if (checkNumPipExist(cmd, spliter)) {
        isNumCmd = true;
        parseNumPipeCmd(newcmd);
      } else {
        launchCmds.push_back(newcmd);
      }
    }
  }
}
