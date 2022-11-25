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
//#include "fast5/nanopolish_fast5_io.cpp"
#include <assert.h>
#include <omp.h>
#include <vector>
#include <iostream>
// #include "fast5/nanopolish_fast5_loader.cpp"
#include "fast5/nanopolish_fast5_loader.h"
#include <fstream>
#include <string>
#include "fast5/hjorth_params.cpp"
#include "svm/mysvm.cpp"
#include <slow5/slow5.h>

void load_data(mySVM &s,std::string fn,int label){

    // std::vector<std::string> m_fast5s;
    // std::ifstream f(fn);
    // std::string line;
    // while(!f.eof()){
    //  std::getline(f,line);
    //  if(line.size()>0)
    //  m_fast5s.push_back(line);
    // }
    // f.close();

    //slow5tools merge can be used for merging files into one file so no need of these lists of files above

    //a simple single threaded slow5 read just for testing - not for performance

    slow5_file_t *sp = slow5_open(fn.c_str(),"r");
    if(sp==NULL){
        fprintf(stderr,"Error in opening file\n");
        exit(1);
    }

    slow5_rec_t *rec = NULL;
    int ret=0;

    while((ret = slow5_get_next(&rec,sp)) >= 0){
        hjorth_params H;
        std::string read_name = rec->read_id;
        Fast5Data data;
        data.is_valid = true;
        data.read_name = read_name;

        // raw data
        fast5_raw_scaling scaling;
        scaling.digitisation = rec->digitisation;
        scaling.offset = rec->offset;
        scaling.range = rec->range;
        scaling.sample_rate = rec->sampling_rate;

        float *rawptr = (float*)calloc(rec->len_raw_signal, sizeof(float)); //TODO must be freed later - not sure where this rawtbl was freed in the fast5 implementation
        raw_table rawtbl = (raw_table) { rec->len_raw_signal, 0, rec->len_raw_signal, rawptr };

        float raw_unit = scaling.range / scaling.digitisation;
        for (size_t i = 0; i < rec->len_raw_signal; i++) {
            rawptr[i] = (rawptr[i] + scaling.offset) * raw_unit;
        }

        data.channel_params = scaling;
        data.rt = rawtbl;
        if(data.rt.n < MIN_LEN+TAIL) continue;
        H.calc_hjorth(data.rt);
//    for(int i=0;i<data.rt.n;i++)
//	cout<<data.rt.raw[i]<<",";
        //cout<<endl;
        if(H.valid_data)
        s.push_data(H,label);
    // for(int l=0;l<H.features.size();l++)
        // 		cout<<H.features[l]<<",";
    // cout<<endl;
    }
    if(ret != SLOW5_ERR_EOF){  //check if proper end of file has been reached
        fprintf(stderr,"Error in slow5_get_next. Error code %d\n",ret);
        exit(1);
    }
    slow5_rec_free(rec);
    slow5_close(sp);


    // for(size_t i = 0; i < m_fast5s.size(); ++i) {

    //     fast5_file f5_file = fast5_open(m_fast5s[i]);
    //     if(!fast5_is_open(f5_file)) {
    //         continue;
    //     }

    //     std::vector<std::string> reads = fast5_get_multi_read_groups(f5_file);

    //     for(size_t j = 0; j < reads.size(); j++) {
    //         hjorth_params H;
	//     assert(reads[j].find("read_") == 0);
    //         std::string read_name = reads[j].substr(5);
    //         Fast5Data data;
    //         data.is_valid = true;
    //         data.read_name = read_name;


    //         // raw data
    //         data.channel_params = fast5_get_channel_params(f5_file, read_name);
    //         data.rt = fast5_get_raw_samples(f5_file, read_name, data.channel_params);
    //         if(data.rt.n < MIN_LEN+TAIL) continue;
    //         H.calc_hjorth(data.rt);
	// //    for(int i=0;i<data.rt.n;i++)
	// //	cout<<data.rt.raw[i]<<",";
    //         //cout<<endl;
    //         if(H.valid_data)
	//      s.push_data(H,label);
	//    // for(int l=0;l<H.features.size();l++)
    //        // 		cout<<H.features[l]<<",";
	//    // cout<<endl;
    //     }

    //     fast5_close(f5_file);
    // }

}
int main(int argc,char *argv[])
{
    mySVM s;
    std::uint16_t no;
    std::string s1(argv[1]);
    if(s1.compare("train")==0){
    std::cout<<"##TRAINING MODE\n";
    std::cout<<"#LOADING TARGET READS.........................\n";
    load_data(s,argv[2],1);
    no=s.data.size();
    std::cout<<"DONE loading\t"<<no<< "valid target reads"<<"\n";
    std::cout<<"\n$$$$$$$$$$$$$$$$$$$$$$$$\n";
    std::cout<<"#LOADING NON_TARGET READ......................\n";
    load_data(s,argv[3],0);

    std::cout<<"DONE loading\t"<<s.data.size()-no<< "valid non-target reads"<<"\n";
    s.train_SVM();}
    else if(s1.compare("test")==0){

    std::cout<<"##TESTING MODE\n";
    std::cout<<"#LOADING TARGET READS.........................\n";
    load_data(s,argv[2],1);
    no=s.data.size();
    std::cout<<"DONE loading\t"<<no<< "valid target reads"<<"\n";

    std::cout<<"#LOADING NON_TARGET READ......................\n";
    load_data(s,argv[3],0);

    std::cout<<"DONE loading\t"<<s.data.size()-no<< "valid non-target reads"<<"\n";
    s.test_SVM(argv);

    }
    else{
    cout<<"ERROR in input format\n";
    exit(1);
    }
return 0;
}
