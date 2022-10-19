#include <iostream>
#include <set>
#include <string>
#include <vector>

class Parser {
private:
  std::set<std::string> bulidInCmd = {"printenv", "setenv", "exit"};
  std::vector<std::pair<int, std::string>>
      numPipeCmds; // save pipe cmd and cold down value.
  std::vector<std::string> launchCmds;

public:
  Parser() {}
  ~Parser() = default;

  bool checkPipExist(std::string &cmd); // check pip
  bool checkNumPipExist(std::string &cmd,
                        std::vector<std::string> &num); // check number pip
  // std::vector<std::string> splitPipCmd();
  std::vector<std::string>
  pipCmdSplit(std::string &cmd); // split cmd form command with pipe
  bool checkBuildin(std::string &cmd);
  void parse(std::string &cmd);
  void numberPipeCmdDeal(std::string &cmd,
                         std::vector<std::string> &numPipeSet);
  void numberCmdCheck(std::string &cmd);
  void launchPipCmd(bool &flag);
  std::vector<std::string> findNumPattern(std::string &cmd,
                                          std::vector<std::string> &num);
  void oneLineNumDeal(std::vector<std::pair<int, std::string>> &oneline);
};
