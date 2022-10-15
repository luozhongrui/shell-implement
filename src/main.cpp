#include "env.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>
#include <vector>
int main() {
  initEnv();
  std::string command;
  while (true) {
    std::cout << "% ";
    getline(std::cin, command);
    if (command.size() > 0) {
      Parser *parser = new Parser(command);
      parser->parse();
    }
  }
}