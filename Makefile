CC = g++
CFLAGS = -Wall -std=c++11 -O3 -DUSE_OPENCV_GRID_SEARCH_AUTOTRAIN -DMED_VAR
SRCS = RawMap.cpp
PROG = RawMap
LIBS = -lz -lm -lstdc++ -ldl  -I/usr/include/opencv -I slow5lib/include/ `pkg-config opencv --cflags --libs`

$(PROG):$(SRCS) slow5lib/lib/libslow5.a
	$(CC) $(CFLAGS) $^ slow5lib/lib/libslow5.a $(LIBS) -o $@
clean:
	-rm $(PROG)
	make -C slow5lib clean

slow5lib/lib/libslow5.a:
	$(MAKE) -C slow5lib zstd=$(zstd) no_simd=$(no_simd) zstd_local=$(zstd_local) lib/libslow5.a