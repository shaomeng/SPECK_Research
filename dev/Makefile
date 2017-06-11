ARCH=$(shell uname)
HOSTNAME=$(shell hostname)
USER=$(shell whoami)

ifeq ($(ARCH), Darwin)
  CC=clang
  CXX=clang++
	CFLAGS=-Wno-c++11-extensions
  LDFLAGS=
else
endif

speck: wavelet speck.cpp speck.h
	$(CXX) $(CFLAGS) bin/wavelet.o speck.cpp -o bin/speck

wavelet: wavelet.cpp wavelet.h
	$(CXX) $(CFLAGS) -c wavelet.cpp -o bin/wavelet.o

