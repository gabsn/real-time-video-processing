SYSTEMC ?= /opt/systemc-2.3.1
LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}:/opt/systemc-2.3.1/lib-linux64
ARCH = linux64
CXX = g++
CC = g++ 

CPPFLAGS  =  -isystem $(SYSTEMC)/include
CXXFLAGS  =  -Wall -g
LDFLAGS   =  -L$(SYSTEMC)/lib-$(ARCH)
LDLIBS    =  -lsystemc

EXE = travail_rtl 
SRC = $(patsubst %, %.cpp, $(EXE))
OBJ = $(patsubst %.cpp, %.o, $(SRC))

all: $(EXE)

$(EXE): $(OBJ) 

run:
	make && ./$(EXE)

clean:
	-rm -f *.o 

.PHONY: clean
