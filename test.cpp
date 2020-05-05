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
#include "fast5/nanopolish_fast5_io.cpp"
#include <assert.h>
#include <omp.h>
#include <vector>
#include <iostream>
#include "fast5/nanopolish_fast5_loader.cpp"
#include <fstream>
#include <string>
#include "fast5/hjorth_params.cpp"
#include "svm/svm.cpp"
void load_data(mySVM &s,std::string fn,int label){

    std::vector<std::string> m_fast5s;
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


        std::vector<std::string> reads = fast5_get_multi_read_groups(f5_file);
        
        for(size_t j = 0; j < reads.size(); j++) {
            hjorth_params H;
	    assert(reads[j].find("read_") == 0);
            std::string read_name = reads[j].substr(5);
            Fast5Data data;
            data.is_valid = true;
            data.read_name = read_name;


            // raw data
            data.channel_params = fast5_get_channel_params(f5_file, read_name);
            data.rt = fast5_get_raw_samples(f5_file, read_name, data.channel_params);
            if(data.rt.n < MIN_LEN) continue;
            H.calc_hjorth(data.rt);
            s.push_data(H,label);
        }

        fast5_close(f5_file);
    }

}
int main(int argc,char *argv[])
{
    mySVM s;
    std::uint16_t no;
    std::cout<<"LOADING TARGET READS.........................\n";
    load_data(s,argv[1],1);
    no=s.samples.size();
    std::cout<<"DONE loading\t"<<no<< "valid target reads"<<"\n";
      
    std::cout<<"LOADING NON_TARGET READ......................\n";
    load_data(s,argv[2],-1);

    std::cout<<"DONE loading\t"<<s.samples.size()-no<< "valid non-target reads"<<"\n";
    s.train_SVM();
return 0;
}
