ARCH=$(shell uname)
HOSTNAME=$(shell hostname)
USER=$(shell whoami)

ifeq ($(ARCH), Darwin)
  QCC_INSTALL=/Users/shaomeng/Install/QccPack-0.61
  CC=clang
  CXX=clang++
  CFLAGS=-g -Wall -std=c++11
  LDFLAGS=
else
  QCC_INSTALL=/Users/shaomeng/Install/QccPack-0.61
  CC=gcc
  CXX=g++
  CFLAGS=-g -Wall -std=c++11
  LDFLAGS=
endif

speckhelper.o: SpeckHelper.cpp SpeckHelper.h
	$(CXX) $(CFLAGS) -c SpeckHelper.cpp -o speckhelper.o

wavelets.o: Wavelets.cpp Wavelets.h
	$(CXX) $(CFLAGS) -c Wavelets.cpp -o wavelets.o

wavelets: Wavelets.cpp Wavelets.h
	$(CXX) $(CFLAGS) Wavelets.cpp -o wavelets

bitbuffer.o: BitBuffer.cpp BitBuffer.h
	$(CXX) $(CFLAGS) -c BitBuffer.cpp -o bitbuffer.o

bitbuffer: BitBuffer.cpp BitBuffer.h
	$(CXX) $(CFLAGS) BitBuffer.cpp -o bitbuffer

