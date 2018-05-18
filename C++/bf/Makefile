CXX=g++
FLAGS=-std=c++0x -O3

TARGETS=bf
all:$(TARGETS)

bf:src/interpreter.cc src/jit_compiler_x64.cc src/timer.cc src/main.cc
	$(CXX) $(FLAGS) -o bf src/interpreter.cc src/timer.cc src/jit_compiler_x64.cc src/main.cc

.PHONY:clean
clean:
	-rm $(TARGETS)

