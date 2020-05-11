<p align="center"> 
<img src="logo.png">
</p>


**Compile and build**:  
make -j <no_of_threads>;

**Training**:
``````
>./RawMap train <file_list_to_target_reads> <file_list_to_non-target_reads>
```````
Model is saved to model.data by default after training.  
**Testing**:
```````
>./RawMap test <file_list_to_target_reads> <file_list_to_non-target_reads> <model.data file name>
```````

**Output**:  
Will result in accuracy, true positive and true negative rates.

**Dependencies: (pre-installed on mbit1)**  
Tested on OpenCV version 2.4.5 (2.4.5) for CentOS, 
libhdf5,[HDF5](https://www.hdfgroup.org/downloads/hdf5/) must be installed. 


Libraries adapted:  
fast5/, used for rawdata extraction, is a modified subset of  files originally from https://github.com/jts/nanopolish


MacOS:  
compile flags in makefile needs to be re-ordered to make this work.

```````
__________________________________________________________________________________________________________________
```````
**INTERNAL:::Repo links on mbit1.eecs.umich.edu**  

Minimap classified reads (To test load c_model.data  )  
separate zymo and hela runs: /y/hariss/FAST5/FAST5  
99:1: /y/hariss/FAST5/99-1  
50:50: /y/hariss/FAST5/50:50  


Minimap unclassified reads (To test load uc_model.data  )
separate zymo and hela: /z/scratch1/hariss/FAST5/    

Zymo sub-communities: (TBD):  
zymo community split into two sub-communities of 5 species each: /z/scratch1/hariss/FAST5/split    


