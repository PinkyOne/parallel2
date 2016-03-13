CXXFLAGS=-Ofast -Iinclude/ -std=c++11
#-DPRINT_PROGRESS -DALGORITHM_VERBOSE

TARGET=bin/klfm

DIRS=bin obj

all : make_dirs $(TARGET)

make_dirs:
	mkdir -p $(DIRS)

OBJS= \
	obj/Bucket.obj \
	obj/Cell.obj \
	obj/CellList.obj \
	obj/CellMove.obj \
	obj/Id.obj \
	obj/Iteration.obj \
	obj/KLFM.obj \
	obj/Net.obj \
	obj/NetlistHypergraph.obj \
	obj/Partition.obj \
	obj/Pin.obj \
	obj/RandomDistribution.obj \
	obj/Solution.obj \
	obj/main.obj

$(TARGET) : $(DIRS) $(OBJS)
	gcc $(CXXFLAGS) $(OBJS) -o $(TARGET) -lstdc++

obj/main.obj:main.cpp
	gcc $(CXXFLAGS) -c -o $@ $< -lstdc++

obj/%.obj:src/%.cpp
	gcc $(CXXFLAGS) -c -o $@ $< -lstdc++

clean: 
	rm -fr bin obj
