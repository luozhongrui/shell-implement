# **NPShell**
---
The npshell should support the following features:
1. Execution of commands
2. Ordinary Pipe
3. Numbered Pipe
4. File Redirection
Project directory structure:
```
working dir
├── Makefile
├── README.md
├── bin
│   ├── noop.cpp
│   ├── number.cpp
│   ├── removetag.cpp
│   └── removetag0.cpp
├── src
│   ├── Makefile
│   ├── include
│   │   ├── createProcess.hpp
│   │   ├── env.hpp
│   │   └── parser.hpp
│   ├── lib
│   │   ├── createProcess.cpp
│   │   ├── env.cpp
│   │   └── parser.cpp
│   └── main.cpp
└── test.html
```
[Usage:] buildin command: printenv setenv
for example: 
```
% printenv PATH
bin:.
% setenv PATH bin
```            