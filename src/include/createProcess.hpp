#include <signal.h>
#include <string>
#include <vector>
void sigHandler(int signo);

class CreateProcess {
private:
  std::string command;
  int pipNum;
  std::vector<std::string> cmdSet; // pip cmd split result
  std::vector<pid_t> pidSet;
  std::vector<std::vector<int>>
      numberPipRecord; // reocrd the number need to wait

public:
  CreateProcess() {}
  CreateProcess(std::vector<std::string> _cmdSet) : cmdSet(_cmdSet) {}
  ~CreateProcess() = default;
  void initalNumber(std::vector<int> number);
  void create(std::string &cmd);                // one command
  void create(std::vector<std::string> pipCmd); // pipe command
  void createPipeProcess(std::string &cmd, std::vector<int *> &pipes, int num);
  void prepareArgue(std::string &cmd, bool &redirect,
                    std::vector<std::string> &args, std::string &fileName);
};
