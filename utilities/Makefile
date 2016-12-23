ARCH=$(shell uname)
HOSTNAME=$(shell hostname)

ifeq ($(ARCH), Darwin)
  CC=clang
  CXX=clang++
  QCC_DIR=/Users/shaomeng/Install/QccPack-0.61
else
  CC=gcc
  CXX=g++
  QCC_DIR=/glade/u/home/shaomeng/lib_gnu/QccPack-0.61
endif

INC_DIR=$(QCC_DIR)/include
LIB_DIR=$(QCC_DIR)/lib

raw2faces: raw2faces.cpp
	$(CXX) -std=c++11 -o bin/raw2faces raw2faces.cpp

raw2pyramid3d: raw2pyramid3d.c
	$(CC) -std=c99 -o bin/raw2pyramid3d -I$(INC_DIR) -L$(LIB_DIR) -lQccPack raw2pyramid3d.c

pyramid3dtest: pyramid3dtest.c
	$(CC) -std=c99 -o bin/pyramid3dtest -I$(INC_DIR) -L$(LIB_DIR) -lQccPack pyramid3dtest.c



