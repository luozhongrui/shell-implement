#include "createProcess.hpp"
#include <iostream>
#include <set>
#include <string>
#include <vector>
struct numPipEntry {
  // int weight;
  int cold;
  std::string cmd;
  std::string typePipe;
  bool flag; // if true
  numPipEntry() : cold(), cmd(), typePipe(), flag() {}
  numPipEntry(int _cold, std::string _cmd, std::string _typePipe, bool _flag)
      : cold(_cold), cmd(_cmd), typePipe(_typePipe), flag(_flag) {}
};
class Parser {
private:
  std::set<std::string> bulidInCmd = {"printenv", "setenv", "exit"};
  std::vector<numPipEntry *> storge; // save pipe cmd and cold down value.
  std::vector<std::string> launchCmds;

public:
  Parser() {}
  ~Parser() = default;

  bool checkPipExist(std::string &cmd); // check pip
  bool checkNumPipExist(std::string &cmd,
                        std::vector<std::string> &num); // check number pip
  std::vector<std::string>
  pipCmdSplit(std::string &cmd); // split cmd form command with pipe
  bool checkBuildin(std::string &cmd);
  void parse(std::string &cmd);

  void launchNumCmd(bool &launched, CreateProcess *creater);
  std::vector<std::string> splitNumPoint(std::string &cmd,
                                         std::vector<std::string> &num);

  void parseNumPipeCmd(std::string &cmd);
  void checkStorge(std::string cmd, bool &isNumCmd);
};
