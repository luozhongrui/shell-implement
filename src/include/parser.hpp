#include <iostream>
#include <set>
#include <string>
#include <vector>

class Parser {
private:
  std::string cmd;
  std::set<std::string> bulidInCmd = {"printenv", "setenv", "exit"};
  bool ifBuildin;
  int num;
  std::vector<std::string> cmdSet;

public:
  Parser(std::string _cmd) : cmd(_cmd) {}
  ~Parser() = default;
  std::vector<std::string> split(const std::string &str,
                                 const std::string &pattern);
  bool checkPipExist();    // check pip
  bool checkNumPipExist(); // check number pip
  std::vector<std::string> splitPipCmd();
  bool checkBuildin();
  void parse();
};
