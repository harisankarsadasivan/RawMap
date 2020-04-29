//---------------------------------------------------------
// Copyright 2016 Ontario Institute for Cancer Research
// Written by Jared Simpson (jared.simpson@oicr.on.ca)
//---------------------------------------------------------
//
// nanopolish_fast5_processor -- framework for iterating
// over a collection of fast5 files and performing some
// action on each read in parallel
//
//#include "nanopolish_common.h"
#include "fast5/nanopolish_fast5_io.h"
#include "fast5/nanopolish_fast5_io.cpp"
#include <assert.h>
#include <omp.h>
#include <vector>
#include <iostream>
#include "fast5/nanopolish_fast5_loader.h"
#include <fstream>
#include <string>

int main()
{
    std::vector<std::string> m_fast5s;
    std::string fn ="fast5_filename.txt";
    std::ifstream f(fn);
    std::string line;
    while(!f.eof()){
     std::getline(f,line);
     if(line.size()>0)
     m_fast5s.push_back(line);
    }
    f.close();

    for(size_t i = 0; i < m_fast5s.size(); ++i) {
        fast5_file f5_file = fast5_open(m_fast5s[i]);
        if(!fast5_is_open(f5_file)) {
            continue;
        }

        std::vector<Fast5Data> fast5_data;

        std::vector<std::string> reads = fast5_get_multi_read_groups(f5_file);
        
        for(size_t j = 0; j < reads.size(); j++) {
            // groups have names like "read_<uuid>"
            // we're only interested in the uuid bit
            assert(reads[j].find("read_") == 0);
            std::string read_name = reads[j].substr(5);
            Fast5Data data;
            data.is_valid = true;
            data.read_name = read_name;

            // metadata
            data.sequencing_kit = fast5_get_sequencing_kit(f5_file, read_name);
            data.experiment_type = fast5_get_experiment_type(f5_file, read_name);

            // raw data
            data.channel_params = fast5_get_channel_params(f5_file, read_name);
            data.rt = fast5_get_raw_samples(f5_file, read_name, data.channel_params);
            data.start_time = fast5_get_start_time(f5_file, read_name);
            for(std::uint32_t i=0;i<data.rt.n;i++)
               std::cout<<data.rt.raw[i]<<"\n";
            fast5_data.push_back(data);
        }

        fast5_close(f5_file);


        // destroy fast5 data
        for(size_t j = 0; j < fast5_data.size(); ++j) {
            free(fast5_data[j].rt.raw);
            fast5_data[j].rt.raw = NULL;
        }
        fast5_data.clear();
    }
return 0;
}
