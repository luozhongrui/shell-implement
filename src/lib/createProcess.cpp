#include "createProcess.hpp"
#include "env.hpp"
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
void CreateProcess::create() {
  // file rediriction need to do
  if (cmdSet.size() != 0) {
    // create pipe if need to
    // TO DO
  } else {
    // one process
    createOneCommand();
  }
}
void CreateProcess::terminate() {}

void CreateProcess::createOneCommand() {
  std::vector<std::string> cmdSeq = split(cmd, " ");

  bool redirect = false;
  int num = cmdSeq.size() + 1;
  std::string fileNmae;
  if (std::count(cmdSeq.begin(), cmdSeq.end(), ">")) {
    redirect = true;
    num = cmdSeq.size() - 1;
  }
  char *arg[num];
  for (size_t i = 0; i < cmdSeq.size(); i++) {
    if (cmdSeq[i] == ">") {
      if (i + 1 < cmdSeq.size()) {
        fileNmae = cmdSeq[i + 1];
      } else {
        std::cerr << "parse error near '\n'" << std::endl;
        return;
      }
      break;
    }
    arg[i] = const_cast<char *>(cmdSeq[i].c_str());
  }
  arg[num - 1] = NULL;

  int pid = fork();
  switch (pid) {
  case -1: {
    std::cerr << "fork fail!" << std::endl;
    break;
  }
  case 0: {
    // child
    if (redirect) {
      int fd = open(fileNmae.c_str(), O_WRONLY | O_CREAT,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      if (fd != STDOUT_FILENO) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
      int statusCode = execvp((cmdSeq[0].c_str()), arg);
      if (statusCode == -1) {
        std::string msg = "Unknown command: [" + cmdSeq[0] + "]";
        std::cerr << msg << std::endl;
        exit(-1);
      }
    } else {
      int statusCode = execvp((cmdSeq[0].c_str()), arg);
      if (statusCode == -1) {
        std::string msg = "Unknown command: [" + cmdSeq[0] + "]";
        std::cerr << msg << std::endl;
        exit(-1);
      }
    }
  }
  default:
    wait(NULL);
    // std::cout<< "finsish child!"<<std::endl;
  }
}