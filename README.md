<p align="center"> 
<img src="logo.png">
</p>  
MIT License

Copyright (c) 2020 harisankarsadasivan

**Description**   
Please refer ONT London Calling poster: <to be updated> .  
RawMap is a direct squiggle-space metagenomic classifier which complements Minimap2 for filtering non-targeted reads. Inspired from brain EEG characterization, RawMap uses a Support Vector Machine (SVM) with an RBF kernel, which is trained to capture the non-linear and non-stationary characteristics of the nanopore squiggles. Each normalized squiggle segment y corresponding to  450 basepairs of a read is mapped to a 3-D feature space. Features are derived from a modified version of Hjorth parameters, where the mean and standard deviation are replaced with median and median absolute deviation respectively. RawMap has two SVM models, one trained on fast and other on slow moving reads.  
**RawMap operates in 3 stages.**
1. FAST5 data extraction and channel normalization followed by MED-MAD scaling to remove any outliers outside 5*MAD:  
2. Modified Hjorth parameter extraction (using MAD instead of STD_DEV)  
3. C_SVC SVM classifier with RBF kernel.  

**Compile and build**:  
make -j <no_of_threads>;

**Training**:
``````
>./RawMap train <file_list_to_target_reads> <file_list_to_non-target_reads>
```````
Command to generate filelist: find <repo_path> type -f | shuf > <file_list>  
Model is saved to model.data by default after training.   
**Testing**:
```````
>./RawMap test <file_list_to_target_reads> <file_list_to_non-target_reads> <model.data file name>
```````

**Output**:  
Will result in accuracy, true positive and true negative rates.

**Dependencies:**  
Tested on OpenCV version 2.4.5 (2.4.5) for CentOS, 
libhdf5,[HDF5](https://www.hdfgroup.org/downloads/hdf5/) must be installed. 
You may use install_hdf5.sh to help with HDF5 installation.


Libraries adapted:  
fast5/, used for rawdata extraction, is a modified subset of  files originally from https://github.com/jts/nanopolish


MacOS:  
compile flags in makefile needs to be re-ordered to make this work.

```````
__________________________________________________________________________________________________________________
```````
**Datasets available at https://doi.org/10.5281/zenodo.7349378**  

**Model files provided**
c_model.data: Trained on Minimap2 classified reads.
uc_model.data: Trained on Minimap2 unclassified reads.

**Training model tuning**    
For the best results, a model needs to be trained for each wet-lab protocol.  
View train.sh for picking the right model.  
