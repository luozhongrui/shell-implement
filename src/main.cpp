#include "env.hpp"
#include "parser.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
int main() {
  initEnv();
  Parser *parser = new Parser();
  std::string command;
  while (true) {
    std::cout << "% ";
    // write(STDOUT_FILENO, "% ", strlen("% "));
    getline(std::cin, command);
    if (command.size() > 0) {
      parser->parse(command);
    }
  }
  delete parser;
  return 0;
}