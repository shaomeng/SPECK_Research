ARCH=$(shell uname)
HOSTNAME=$(shell hostname)

ifeq ($(ARCH), Darwin)
  CC=clang
  CXX=clang++
  QCC_DIR=/Users/shaomeng/Install/QccPack-0.61
  INC_DIR=$(QCC_DIR)/include
  LIB_DIR=$(QCC_DIR)/lib
else
  CC=gcc
  CXX=g++
  INC_DIR=/glade/u/home/shaomeng/lib_gnu/include
  LIB_DIR=/glade/u/home/shaomeng/lib_gnu/lib
endif


raw2faces: raw2faces.cpp
	$(CXX) -std=c++11 -o bin/raw2faces raw2faces.cpp

raw2WAVSubbandPyramid3D: raw2WAVSubbandPyramid3D.c
	$(CC) -o bin/raw2WAVSubbandPyramid3D -I$(INC_DIR) -L$(LIB_DIR) -lQccPack raw2WAVSubbandPyramid3D.c
