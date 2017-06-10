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

lifting: lifting_cdf9_7.cpp
	$(CXX) $(CFLAGS) lifting_cdf9_7.cpp -o bin/lifting 

