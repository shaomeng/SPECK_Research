ARCH=$(shell uname)
HOSTNAME=$(shell hostname)
USER=$(shell whoami)

ifeq ($(ARCH), Darwin)
  CC=clang
  CXX=clang++
  CFLAGS=-g -Wall -std=c++11
  LDFLAGS=
else
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

waveletpyramid.o: WaveletPyramid.cpp Wavelets.h
	$(CXX) $(CFLAGS) -c WaveletPyramid.cpp -o waveletpyramid.o

clean:
	rm -f speckhelper.o wavelets.o wavelets bitbuffer.o bitbuffer test_buffer.bitstream waveletpyramid.o
