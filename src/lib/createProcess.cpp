#include "createProcess.hpp"
#include "env.hpp"
#include <algorithm>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void sigHandler(int signo) {
  int stat;
  while (waitpid(-1, &stat, WNOHANG) > 0)
    ;
}

void execWait(pid_t pid) {
  int wait_pid;
  while (1) {
    wait_pid = waitpid(
        pid, NULL, WNOHANG); // if child not yet finish -> return 0 , otherwise
                             // return child_pid , WNOHANG: won't wait(no hang)
    if (wait_pid == pid)     // success finish
      break;
  }
  return;
}

void CreateProcess::create(std::string &cmd) {
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
      int fd = open(fileNmae.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      // int fd =
      //     open(fileNmae.c_str(), O_CREAT | O_TRUNC | O_RDWR | O_RSYNC, 0666);
      if (fd != STDOUT_FILENO) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
    }
    int statusCode = execvp(arg[0], arg);
    if (statusCode == -1) {
      std::string msg = "Unknown command: [" + cmdSeq[0] + "].\n";
      // std::cerr << msg << std::endl;
      write(STDERR_FILENO, msg.c_str(), strlen(msg.c_str()));
      exit(-1);
    }
  }
  default:
    wait(NULL);
    // std::cout<< "finsish child!"<<std::endl;
  }
}

void CreateProcess::createPipe(std::string &cmd) {
  // signal(SIGCHLD, sigHandler);
  std::vector<std::string> decol;
  std::vector<std::string> pipCmd;
  pipCmd = splitPipeAndError(cmd, decol);

  int pipeNum = pipCmd.size() - 1;
  std::vector<int *> pipes;
  // create all pipe
  for (int i = 0; i < pipeNum; i++) {
    int *pfd = new int[2];
    pipe(pfd);
    pipes.push_back(pfd);
  }
  // fork process
  for (size_t cmdNum = 0; cmdNum < pipCmd.size(); cmdNum++) {
    // child process
    createPipeProcess(pipCmd[cmdNum], pipes, cmdNum, decol);
  }
  for (int i = 0; i < pipeNum; i++) {
    // close(pipes[i][0]);
    // close(pipes[i][1]);
    delete[] pipes[i];
  }
}

void CreateProcess::createPipeProcess(std::string &cmd,
                                      std::vector<int *> &pipes, int num,
                                      std::vector<std::string> &decl) {
  bool redirect;
  std::vector<std::string> referens;
  std::string fileName = "";
  prepareArgue(cmd, redirect, referens, fileName);
  char *arges[referens.size() + 1];
  for (int i = 0; i < int(referens.size()); i++) {
    arges[i] = const_cast<char *>(referens[i].c_str());
  }
  arges[referens.size()] = NULL;

label1:
  pid_t childPid = fork();
  if (childPid == -1) {
    // execWait(childPid);
    sigHandler(0);
    goto label1;
  }
  switch (childPid) {
  case 0: {
    for (int i = 0; i < num - 1; i++) {
      close(pipes[i][0]);
      close(pipes[i][1]);
    }
    for (int i = num + 1; i < int(pipes.size()); i++) {
      close(pipes[i][0]);
      close(pipes[i][1]);
    }
    if (num == 0) {
      close(pipes[num][0]); // read
      if (pipes[num][1] != STDOUT_FILENO) {
        dup2(pipes[num][1], STDOUT_FILENO); // write
      }
      if (decl[num] == "!") {
        if (pipes[num][1] != STDERR_FILENO) {
          dup2(pipes[num][1], STDERR_FILENO); // write
        }
      }
      close(pipes[num][1]);
    }
    if (num == int(pipes.size())) { // last cmd
      close(pipes[num - 1][1]);     // write
      if (pipes[num - 1][0] != STDIN_FILENO) {
        dup2(pipes[num - 1][0], STDIN_FILENO);
        close(pipes[num - 1][0]);
      }
    }
    if (num != 0 && num != int(pipes.size())) {
      // prev pipe
      close(pipes[num - 1][1]);
      if (pipes[num - 1][0] != STDIN_FILENO) {
        dup2(pipes[num - 1][0], STDIN_FILENO);
        close(pipes[num - 1][0]);
      }
      // next pipe
      close(pipes[num][0]);
      if (pipes[num][1] != STDOUT_FILENO) {
        dup2(pipes[num][1], STDOUT_FILENO);
      }
      if (decl[num] == "!") {
        if (pipes[num][1] != STDERR_FILENO) {
          dup2(pipes[num][1], STDERR_FILENO);
        }
      }
      close(pipes[num][1]);
      // last if
    }
    if (redirect) {
      int fd = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      if (fd != STDOUT_FILENO) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
    }
    int statusCode = execvp(arges[0], arges);
    if (statusCode == -1) {
      std::string command = arges[0];
      std::string msg = "Unknown command: [" + command + "].\n";
      write(STDERR_FILENO, msg.c_str(), strlen(msg.c_str()));
      exit(-1);
    }
    // case 0;
  }
  default: {
    if (num - 2 >= 0) {
      close(pipes[num - 2][0]);
      close(pipes[num - 2][1]);
    }
    if (num == int(pipes.size())) {
      for (int i = 0; i < num; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
      }
      execWait(childPid);
    }
  }
  }
}

void CreateProcess::prepareArgue(std::string &cmd, bool &redirect,
                                 std::vector<std::string> &args,
                                 std::string &fileName) {
  std::vector<std::string> cmdSeq = split(cmd, " ");
  redirect = false;
  if (std::count(cmdSeq.begin(), cmdSeq.end(), ">")) {
    redirect = true;
  }
  for (size_t i = 0; i < cmdSeq.size(); i++) {
    if (cmdSeq[i] == ">") {
      if (i + 1 < cmdSeq.size()) {
        fileName = cmdSeq[i + 1];
      } else {
        std::cerr << "parse error near '\n'" << std::endl;
        return;
      }
      break;
    }
    // arg[i] = const_cast<char *>(cmdSeq[i].c_str());
    args.push_back(cmdSeq[i]);
  }
}

// void CreateProcess::createErrorPipe(std::string &cmd) {}