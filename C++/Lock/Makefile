BINS=bakery

all: $(BINS)
bakery:bakery.cc
	g++ -std=c++17 -o $@ $< -lpthread

.PHONY:clean
clean:
	rm $(BINS)
