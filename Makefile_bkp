CC = g++
CFLAGS = -Wall -std=c++11 -O3 -DSIMPLE_OPT
SRCS = RawMap.cpp svm/svm.cpp
PROG = RawMap
#OPENCV = `pkg-config opencv --cflags --libs`
LIBS = -lz -lm -lstdc++ -ldl -lhdf5 -I/home/hariss/hari/RawMap/dlib-19.19 /home/hariss/hari/RawMap/dlib-19.19/dlib/all/source.cpp -lpthread -lX11

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS) $(INC)
clean:
	-rm $(PROG)
