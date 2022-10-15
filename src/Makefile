CXX := g++
CXXFLAGS := -g -std=c++11 -Wall
INCLUDE := -Iinclude

.PHONY:  clean

SRCDIR = lib/
SRC := $(shell find $(SRCDIR) -name '*.cpp')

objects = $(SRC) main.cpp

npshell: $(objects)
	$(CXX) $(CXXFLAGS) -o ../npshell $(objects) $(INCLUDE)


clean :
	-rm ../npshell

