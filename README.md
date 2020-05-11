<p align="center"> 
<img src="logo.png">
</p>


**Compile and build:
make -j 4;

**Training:
``````
>./RawMap train <file_list_to_target_reads> <file_list_to_non-target_reads>
```````
**Testing:
```````
>./RawMap test <file_list_to_target_reads> <file_list_to_non-target_reads> <model.data file name>
```````

#Output:
Will result in accuracy, true positive and true negative rates.

**Dependencies: (pre-installed on mbit1)**
OpenCV version 2.4.5 (2.4.5) for CentOS, 
libhdf5

Adapted:
fast5/, used for rawdata extraction, is a modified subset of  files originally from https://github.com/jts/nanopolish
cvsvm from OpenCV

Note: compile flags in makefile needs to be re-ordered to make this work in macos.
