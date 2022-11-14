#include <signal.h>
#include <string>
#include <vector>
const int MAXNUM = 512;
void sigHandler(int signo);
void execWait(pid_t pid);
class CreateProcess {
private:
public:
  CreateProcess() { /*signal(SIGCHLD, sigHandler);*/
  }
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
