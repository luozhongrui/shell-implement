#include "createProcess.hpp"
#include "env.hpp"
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
    std::vector<std::string> cmdSeq = split(cmd, " ");
    const int num = cmdSeq.size()+1;
    char *arg[num];
    for (size_t i = 0; i < cmdSeq.size(); i++) {
      arg[i] = const_cast<char *>(cmdSeq[i].c_str());
      
    }
    arg[cmdSeq.size()] = NULL;

    int pid = fork();
    switch (pid) {
    case -1: {
      std::cout << "fork fail!" << std::endl;
      break;
    }
    case 0: {
      // child
      int statusCode = execvp((cmdSeq[0].c_str()), arg);
      if (statusCode == -1) {
        std::string msg = "Unknown command: [" + cmdSeq[0] + "]";
          std::cerr << msg<< std::endl;
        exit(-1);
      exit(0);
      }
    }
    default:
      wait(NULL);
      // std::cout<< "finsish child!"<<std::endl;
    }
  }
}
void CreateProcess::terminate() {}