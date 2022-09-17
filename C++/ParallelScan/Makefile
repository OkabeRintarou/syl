BINS=scan_test perf_test

all:$(BINS)

scan_test:main.cu timer.cpp util.cpp
	g++ -Werror -O3 -fopenmp -o scan_test main.cpp timer.cpp util.cpp

perf_test:perf_test.cpp timer.cpp util.cpp
	g++ -Werror -O3 -fopenmp -o perf_test perf_test.cpp timer.cpp util.cpp

.PHONY:clean
clean:
	rm $(BINS)
