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

smallimagecubes2raw: smallimagecubes2raw.c
	$(CC) -O3 -std=c99 -o bin/smallimagecubes2raw -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) smallimagecubes2raw.c

raw2smallimagecubes: raw2smallimagecubes.c
	$(CC) -O3 -std=c99 -o bin/raw2smallimagecubes -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) raw2smallimagecubes.c

raw2imagecube: raw2imagecube.c
	$(CC) -O3 -std=c99 -o bin/raw2imagecube -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) raw2imagecube.c

compareraw: compareraw.c
	$(CC) -std=c99 -o bin/compareraw -lm compareraw.c

raw2faces: raw2faces.cpp
	$(CXX) -std=c++11 -o bin/raw2faces raw2faces.cpp

raw2pyramid3d: raw2pyramid3d.c
	$(CC) -O3 -std=c99 -o bin/raw2pyramid3d -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) raw2pyramid3d.c

pyramid3dtestdwtidwt: pyramid3dtestDwtIdwt.c
	$(CC) -std=c99 -o bin/pyramid3dtestDwtIdwt -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) pyramid3dtestDwtIdwt.c



