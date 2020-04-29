CC = g++
CFLAGS = -Wall -std=c++11 -O3
SRCS = test.cpp
PROG = test
OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV) -lz -lm -lstdc++ -ldl -lhdf5

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) $(INC)
clean:
	-rm $(PROG)
