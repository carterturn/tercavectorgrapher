CPP=g++

build:
	$(CPP) main.cpp --std=c++0x -lGL -lglfw -I/usr/include/ctvector -lctvector
