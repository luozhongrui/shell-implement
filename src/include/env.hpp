#include <string>
#include <vector>
void printEnv(std::string env);

void setEnv(std::string env, std::string value);

void initEnv();

bool checkCmdexist(std::string cmd);

std::vector<std::string> split(const std::string &str,
                               const std::string &pattern);

std::vector<std::string> splitPipeAndError(std::string &cmd,
                                           std::vector<std::string> &spliter);