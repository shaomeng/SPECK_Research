ARCH=$(shell uname)
HOSTNAME=$(shell hostname)
USER=$(shell whoami)

ifeq ($(ARCH), Darwin)
  QCC_INSTALL=/Users/shaomeng/Install/QccPack-0.61
  CC=clang
  CXX=clang++
  CFLAGS=-g -Wno-c++11-extensions
  LDFLAGS=
else
endif

speck: wavelet.o speck.cpp speck.h
	$(CXX) $(CFLAGS) -DHAVE_SPECK -I$(QCC_INSTALL)/include bin/libQccPack.a wavelet.o speck.cpp -o bin/speck

wavelet.o: wavelet.cpp wavelet.h
	$(CXX) $(CFLAGS) -c wavelet.cpp -o wavelet.o

bit_buffer.o: bit_buffer.cpp bit_buffer.h
	$(CXX) $(CFLAGS) -c bit_buffer.cpp -o bit_buffer.o

bit_buffer: bit_buffer.cpp bit_buffer.h
	$(CXX) $(CFLAGS) bit_buffer.cpp -o bit_buffer

