#include <iostream>
#include <set>
#include <string>
#include <vector>

class Parser {
private:
  // std::string command;
  std::set<std::string> bulidInCmd = {"printenv", "setenv", "exit"};
  // bool ifBuildin;
  // std::vector<int> numPipSet;
  // std::vector<std::string> pipeCmdSet; // cmd set from command with pipe

public:
  Parser() {}
  ~Parser() = default;

  bool checkPipExist(std::string &cmd);    // check pip
  bool checkNumPipExist(std::string &cmd); // check number pip
  // std::vector<std::string> splitPipCmd();
  std::vector<std::string>
  pipCmdSplit(std::string &cmd); // split cmd form command with pipe
  bool checkBuildin(std::string &cmd);
  void parse(std::string &cmd);
};
