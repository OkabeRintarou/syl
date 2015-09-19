
CC := clang++
FLAGS := -std=c++11 -Wall

TARGETS := newc rmexe

.PHONY:all
	
all:$(TARGETS)

newc:newc.cpp
	$(CC) $(FLAGS) -o newc newc.cpp
rmexe:rmexe.cpp
	$(CC) $(FLAGS) -o rmexe rmexe.cpp

.PHONY:clean
clean:
	-rm $(TARGETS)
