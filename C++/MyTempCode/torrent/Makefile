CL = clang++
OBJ = main.o BTParser.o BitTorrent.o nettools.cpp
FLAGS = -std=c++0x
HPP = Error.h 
tdown:$(OBJ)
	$(CL) $(FLAGS) $(OBJ) -o tdown
main.o:main.cpp Error.h BTParser.h BitTorrent.h
	$(CL) -c $(FLAGS)  -o main.o main.cpp
BTParser.o:BTParser.cpp BTParser.h Error.h
	$(CL) -c $(FLAGS)  -o BTParser.o BTParser.cpp
BitTorrent.o:BitTorrent.cpp BitTorrent.h BTParser.h Error.h
	$(CL) -c $(FLAGS)  -o BitTorrent.o BitTorrent.cpp
nettools.o:nettools.cpp nettools.h
	$(CL) -c $(FLAGS) -o nettools.o nettools.cpp
.PHONY:clean

clean:
	-rm $(OBJ) tdown