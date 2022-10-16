#include <string>
#include <vector>

class CreateProcess {
private:
  std::string cmd;
  int pipNum;
  std::vector<std::string> cmdSet; // pip cmd split result
  std::vector<pid_t> pidSet;

public:
  CreateProcess(std::string _cmd) : cmd(_cmd) {}
  CreateProcess(std::vector<std::string> _cmdSet) : cmdSet(_cmdSet) {
    pipNum = cmdSet.size();
  }
  ~CreateProcess() = default;
  void create();
  void createOneCommand();
  void terminate();
  void terminate(pid_t pid);
};
