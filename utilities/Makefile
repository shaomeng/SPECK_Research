ARCH=$(shell uname)
HOSTNAME=$(shell hostname)
USER=$(shell whoami)

ifeq ($(ARCH), Darwin)
  CC=clang
  CXX=clang++
  QCC_DIR=/Users/shaomeng/Install/QccPack-0.61
  LDFLAGS=
else
  ifeq ($(user), shaomeng)
    CC=gcc
    CXX=g++
    QCC_DIR=/glade/u/home/shaomeng/lib_gnu/QccPack-0.61
    LDFLAGS=
  else              # Alaska
    CC=clang
    CXX=clang++
    QCC_DIR=/home/users/samuelli/Install/QccPack-0.61
    LDFLAGS=-lm -lpthread
  endif
endif

INC_DIR=$(QCC_DIR)/include
LIB_DIR=$(QCC_DIR)/lib

raw2faces: raw2faces.cpp
	$(CXX) -std=c++11 -o bin/raw2faces raw2faces.cpp

raw2pyramid3d: raw2pyramid3d.c
	$(CC) -std=c99 -o bin/raw2pyramid3d -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) raw2pyramid3d.c

pyramid3dtestdwtidwt: pyramid3dtestDwtIdwt.c
	$(CC) -std=c99 -o bin/pyramid3dtestDwtIdwt -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) pyramid3dtestDwtIdwt.c



