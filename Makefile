#makefile for the BRE tool
PROGS_HOME = $(PWD)
SRC_D = $(PROGS_HOME)/src
INCLUDE_D = $(PROGS_HOME)/include
BIN_D = $(PROGS_HOME)/bin
LIB_D = $(PROGS_HOME)/lib

ROOT_LIBS = `root-config --libs`
FAIR_LIBS = -lBaseMQ -lBase -lEventDisplay -lfairmq_logger -lFairMQ -lFairMQTest -lFairRutherford -lFairTestDetector -lFairTools -lGeane -lGen -lGeoBase -lLmdMQSampler -lMbsAPI -lMCStack -lParBase -lParMQ -lPassive -lPixel -lTrkBase
R3B_LIBS = -lELILuMon -lField -lR3Bbase -lR3BCalo -lR3BData -lR3BDch -lR3BdTof -lR3BEvtVis -lR3BFi4 -lR3BGen -lR3BGfi -lR3BLand -lR3BLos -lR3BMfi -lR3BmTof -lR3BNeuland -lR3BPassive -lR3BPlist -lR3BPsp -lR3BSTaRTra -lR3BTCal -lR3BTof -lR3BTra -lR3BXBall -ltimestitcher -lR3BMVPlanes
ROOT_LIBS += $(FAIR_LIBS)
ROOT_LIBS += $(R3B_LIBS)
ROOT_FLAGS = `root-config --cflags`
ROOT_FLAGS += -L $(SIMPATH)/lib -L $(FAIRROOTPATH)/lib -I $(SIMPATH)/include -I $(FAIRROOTPATH)/include
CXX_FLAGS = -std=c++11 -I $(INCLUDE_D) -ggdb -fPIC

OBJECTS = bre_reader.o bre_stitch.o bre_data_types.o bre_phyltree.o
LIBS = libbre.a
TEST_PRG = test_reader

#-------------------------------------------------------------------------------------
#objects
bre_reader.o :
	$(CXX) $(SRC_D)/bre_reader.cc $(CXX_FLAGS) $(ROOT_FLAGS) -c -o $(BIN_D)/bre_reader.o

bre_stitch.o :
	$(CXX) $(SRC_D)/bre_stitch.cc $(CXX_FLAGS) $(ROOT_FLAGS) -c -o $(BIN_D)/bre_stitch.o

bre_data_types.o:
	$(CXX) $(SRC_D)/bre_data_types.cc $(CXX_FLAGS) $(ROOT_FLAGS) -c -o $(BIN_D)/bre_data_types.o

bre_phyltree.o:
	$(CXX) $(SRC_D)/bre_phyltree.cc $(CXX_FLAGS) $(ROOT_FLAGS) -c -o $(BIN_D)/bre_phyltree.o

#-------------------------------------------------------------------------------------
#library
libbre.a : $(OBJECTS)
	$(CXX) $(BIN_D)/*.o $(CXX_FLAGS) $(ROOT_FLAGS) $(ROOT_LIBS) -shared -o $(LIB_D)/libbre.a

#-------------------------------------------------------------------------------------
#test programs
test_bre :
	$(CXX) test/test_bre.cpp -L $(PROGS_HOME)/lib -lbre $(CXX_FLAGS) $(ROOT_FLAGS) $(ROOT_LIBS) -o test/test_bre

#-------------------------------------------------------------------------------------
#phony
.PHONY: all
all: $(OBJECTS) $(LIBS)

.PHONY: clean
clean:
	rm -rf $(BIN_D)/*.o $(LIB_D)/*.a
