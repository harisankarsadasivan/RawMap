CC = g++
CFLAGS = -Wall -std=c++11 -O3 -DUSE_OPENCV_GRID_SEARCH_AUTOTRAIN -DMED_VAR
SRCS = RawMap.cpp
PROG = RawMap
LIBS = -lz -lm -lstdc++ -ldl -lhdf5  -I/usr/include/opencv `pkg-config opencv --cflags --libs`

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
clean:
	-rm $(PROG)
