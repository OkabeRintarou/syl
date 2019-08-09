TARGETS=slist dlist_test

all:$(TARGETS)

slist:slist.c
	gcc -O3 -fsanitize=address -o slist slist.c

dlist_test:dlist.c dlist_test.cpp
	g++ -I. -O2 -Wall -o dlist_test dlist.c dlist_test.cpp -lgtest -lpthread

.PHONY:clean

clean:
	-rm ${TARGETS}
