#include <signal.h>
#include <string>
#include <vector>
void sigHandler(int signo);

class CreateProcess {
private:
public:
  CreateProcess() { signal(SIGCHLD, sigHandler); }
  ~CreateProcess() = default;
  void create(std::string &cmd);     // one command
  void createPipe(std::string &cmd); // pipe command
  void createPipeProcess(std::string &cmd, std::vector<int *> &pipes, int num,
                         std::vector<std::string> &decl);
  void prepareArgue(std::string &cmd, bool &redirect,
                    std::vector<std::string> &args, std::string &fileName);

  void createNumCmd(std::vector<std::string> &numPipeCmd);
  void createErrorPipe(std::string &cmd);
};
