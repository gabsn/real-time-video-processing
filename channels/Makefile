SYSTEMC ?= /opt/systemc-2.3.1
LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}:/opt/systemc-2.3.1/lib-linux64
ARCH = linux64
CXX = g++
CC = g++ 

CPPFLAGS  =  -isystem $(SYSTEMC)/include
CXXFLAGS  =  -Wall -g
LDFLAGS   =  -L$(SYSTEMC)/lib-$(ARCH)
LDLIBS    =  -lsystemc

clean:
	-rm -f *.o *.vcd
	-find . -executable -type f -delete

.PHONY: clean
