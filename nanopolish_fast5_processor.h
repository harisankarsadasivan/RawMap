//---------------------------------------------------------
// Copyright 2016 Ontario Institute for Cancer Research
// Written by Jared Simpson (jared.simpson@oicr.on.ca)
//---------------------------------------------------------
//
// nanopolish_fast5_processor -- framework for iterating
// over a collection of fast5 files and performing some
// action on each read in parallel
//
#ifndef NANOPOLISH_FAST5_PROCESSOR_H
#define NANOPOLISH_FAST5_PROCESSOR_H

#include <functional>
#include <string>
#include "nanopolish_read_db.h"
#include "nanopolish_fast5_loader.h"

typedef std::function<void(const Fast5Data& d)> fast5_processor_work_function;

class Fast5Processor
{

    public:

        // process all fast5s in the db
        Fast5Processor(const ReadDB& read_db,
                       const int num_threads,
                       const int batch_size=4000);
        
        // process a single fast5
        Fast5Processor(const std::string& fast5_file,
                       const int num_threads,
                       const int batch_size=4000);

        ~Fast5Processor();

        // process each record in parallel, using the input function
        void parallel_run(fast5_processor_work_function f);

    private:

        std::vector<std::string> m_fast5s;
        int m_batch_size = 4000;
        int m_num_threads = 1;
};

#endif
