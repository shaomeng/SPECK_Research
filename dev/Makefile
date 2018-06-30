ARCH=$(shell uname)
HOSTNAME=$(shell hostname)
USER=$(shell whoami)

ifeq ($(ARCH), Darwin)
  QCC_INSTALL=/Users/shaomeng/Install/QccPack-0.61
  CC=clang
  CXX=clang++
  CFLAGS=-g -Wall -Wno-c++11-extensions
  LDFLAGS=
else
  QCC_INSTALL=/Users/shaomeng/Install/QccPack-0.61
  CC=gcc
  CXX=g++
  CFLAGS=-g -Wall 
  LDFLAGS=
endif

speck: wavelet.o speck.cpp speck.h
	$(CXX) $(CFLAGS) -DHAVE_SPECK -I$(QCC_INSTALL)/include bin/libQccPack.a wavelet.o speck.cpp -o bin/speck

wavelet.o: wavelet.cpp wavelet.h
	$(CXX) $(CFLAGS) -c wavelet.cpp -o wavelet.o

bitbuffer.o: BitBuffer.cpp BitBuffer.h
	$(CXX) $(CFLAGS) -c BitBuffer.cpp -o bitbuffer.o

bitbuffer: BitBuffer.cpp BitBuffer.h
	$(CXX) $(CFLAGS) BitBuffer.cpp -o bitbuffer

