ARCH=$(shell uname)
HOSTNAME=$(shell hostname)

ifeq ($(ARCH), Darwin)
  CC=clang
  CXX=clang++
  CFLAGS=-g -Wall -std=c++11
else
  CC=gcc
  CXX=g++
endif
  
CFLAGS=-g -Wall -std=c++11

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

waveletpyramid: WaveletPyramid.cpp Wavelets.h
	$(CXX) $(CFLAGS) WaveletPyramid.cpp -o waveletpyramid

clean:
	rm -rf speckhelper.o wavelets.o wavelets bitbuffer.o bitbuffer test_buffer.bitstream waveletpyramid.o waveletpyramid *.dSYM
