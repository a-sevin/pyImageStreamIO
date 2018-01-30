CC=g++

CONDA_ROOT   ?= $(HOME)/miniconda3
PYBIND11_ROOT?= $(HRAAPATH)/hraa/lib/pybind11
CACAO_ROOT  ?= $(HOME)/workspace/cacao

LIB_CPPFLAGS  = -W -Wall -fPIC -pedantic -isystem /usr/include/python3.6m --std=c++11
LIB_CPPFLAGS += -isystem $(CONDA_ROOT)/include/python3.6m -isystem $(PYBIND11_ROOT)/include # -isystem $(BOOST_ROOT)/include
LIB_CPPFLAGS += -isystem $(CACAO_ROOT)/src/ImageStreamIO

LIB_DEPLIBS  += -ldl -lrt -lpython3 -lpython3.6m # -lboost_python3 -lboost_numpy3
LIB_DEPLIBS  += $(CACAO_ROOT)/lib/libimagestreamio.la

LIB = pyImageStreamIO

pybind: $(LIB).o
	libtool --mode=link $(CC) --shared $< $(LIB_DEPLIBS) -o $(LIB).so

all: pybind

%.o: %.cpp
	$(CC) $(LIB_CPPFLAGS) -c $<

clean:
	rm *.o $(LIB).so

install_hraa:
	cp pyImageStreamIO.so $(HRAAPATH)/hraa/lib