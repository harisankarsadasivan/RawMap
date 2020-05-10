# RawMap
usage:
Training:
./RawMap <file_list_to_target_reads> <file_list_to_non-target_reads>


Adapted:
fast5/, used for rawdata extraction, is a modified subset of  files originally from https://github.com/jts/nanopolish

Dependencies: (pre-installed on mbit1)
OpenCV
libhdf5

Note: compile flags in makefile needs to be re-ordered to make this work in macos.
