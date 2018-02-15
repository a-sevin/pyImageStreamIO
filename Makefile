CC=g++

CONDA_ROOT    ?= $(HOME)/miniconda3
PYBIND11_ROOT ?= $(HRAAPATH)/hraa/lib/pybind11
CACAO_ROOT    ?= $(HOME)/workspace/cacao

LIB_CPPFLAGS   = -W -Wall -fPIC -pedantic --std=c++11  # -isystem /usr/include/python3.6m
LIB_CPPFLAGS  += -isystem $(CONDA_ROOT)/include/python3.6m -isystem $(PYBIND11_ROOT)/include
LIB_CPPFLAGS  += -isystem $(CACAO_ROOT)/src/ImageStreamIO

LIB_DEPLIBS    = -Wl,-rpath=$(CONDA_ROOT)/lib -L$(CONDA_ROOT)/lib
LIB_DEPLIBS   += -ldl -lrt -lpython3.6m
LIB_DEPLIBS   += $(CACAO_ROOT)/lib/libimagestreamio.la

LIB = pyImageStreamIO

$(LIB).so: $(LIB).o
	$(CACAO_ROOT)/libtool --mode=link $(CC) --shared $< $(LIB_DEPLIBS) -o $(LIB).so

all:  $(LIB).so

%.o: %.cpp
	$(CC) $(LIB_CPPFLAGS) -c $<

clean:
	rm *.o

clean_lib:
	rm $(LIB).so

install_hraa: $(LIB).so
	cp $(LIB).so $(HRAAPATH)/hraa/lib

install_conda: $(LIB).so
	cp $(LIB).so $(CONDA_ROOT)/lib/python3.6/site-packages