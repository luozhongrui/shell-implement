#include <iostream>
#include <set>
#include <string>
#include <vector>

class Parser {
private:
  std::string cmd;
  std::set<std::string> bulidInCmd = {"printenv", "setenv", "exit"};
  bool ifBuildin;
  std::vector<int> numPipSet;
  std::vector<std::string> pipeCmdSet; // cmd set from command with pipe

public:
  Parser(std::string _cmd) : cmd(_cmd) {}
  ~Parser() = default;

  bool checkPipExist();    // check pip
  bool checkNumPipExist(); // check number pip
  // std::vector<std::string> splitPipCmd();
  std::vector<std::string> pipCmdSplit(); // split cmd form command with pipe
  bool checkBuildin();
  void parse();
};
