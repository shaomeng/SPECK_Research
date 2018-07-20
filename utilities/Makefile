ARCH=$(shell uname)
HOSTNAME=$(shell hostname)
USER=$(shell whoami)

ifeq ($(ARCH), Darwin)
  CC=clang
  CXX=clang++
  QCC_DIR=/Users/shaomeng/Install/QccPack-0.61
  CFLAGS= -O3 -std=c99
  LDFLAGS=
else
  CC=gcc
  CXX=g++
  QCC_DIR=/home/shaomeng/Install/QccPack-0.61-1
  CFLAGS= -O3 -std=c99
  LDFLAGS=
endif

INC_DIR=$(QCC_DIR)/include
LIB_DIR=$(QCC_DIR)/lib

dwt3d: dwt3d.c
	$(CC) -o bin/dwt3d -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) dwt3d.c

imagecubeDwtCompression: imagecubeDwtCompression.c
	$(CC) -O3 -std=c99 -o bin/imagecubeDwtCompression -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) imagecubeDwtCompression.c

imagecube2raw: imagecube2raw.c
	$(CC) -O3 -std=c99 -o bin/imagecube2raw $(CFLAGS) -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) imagecube2raw.c

smallimagecubes2raw: smallimagecubes2raw.c
	$(CC) -O3 -std=c99 -o bin/smallimagecubes2raw -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) smallimagecubes2raw.c

raw2smallimagecubes: raw2smallimagecubes.c
	$(CC) -O3 -std=c99 -o bin/raw2smallimagecubes -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) raw2smallimagecubes.c

raw2imagecube: raw2imagecube.c
	#$(CC) -o bin/raw2imagecube $(CFLAGS) -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) raw2imagecube.c
	$(CC) -o bin/raw2imagecube $(CFLAGS) -I$(INC_DIR) $(LDFLAGS) raw2imagecube.c $(QCC_DIR)/lib/libQccPack.a -lm -lpthread

compareraw: compareraw.c
	$(CC) -O3 -std=c99 -o bin/compareraw -lm compareraw.c

compareimagecube: compareimagecube.c
	$(CC) -O3 -std=c99 -o bin/compareimagecube -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) compareimagecube.c

raw2faces: raw2faces.cpp
	$(CXX) -std=c++11 -o bin/raw2faces raw2faces.cpp

raw2pyramid3d: raw2pyramid3d.c
	$(CC) -O3 -std=c99 -o bin/raw2pyramid3d -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) raw2pyramid3d.c

pyramid3dtestdwtidwt: pyramid3dtestDwtIdwt.c
	$(CC) -std=c99 -o bin/pyramid3dtestDwtIdwt -I$(INC_DIR) -L$(LIB_DIR) -lQccPack $(LDFLAGS) pyramid3dtestDwtIdwt.c



