#include "env.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
int main() {
  initEnv();
  std::string command;
  while (true) {
    // std::cout << "% ";
    write(STDOUT_FILENO, "% ", strlen("% "));
    getline(std::cin, command);
    if (command.size() > 0) {

      Parser *parser = new Parser(command);
      parser->parse();
      delete parser;
    }
  }
}