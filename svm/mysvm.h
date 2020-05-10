#pragma once

//#include "svm.h"
#include <ctype.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cv.h>
#include <ml.h>
#include <stdio.h>

using namespace cv;
using namespace std;
#include "../fast5/hjorth_params.h"

//#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

//struct svm_parameter param;		// set by parse_command_line
//struct svm_problem prob;		// set by read_problem
//struct svm_model *model;
//struct svm_node *x_space;

class mySVM{
public:


std::uint16_t N=3;
std::uint32_t data_size;
std::vector<std::vector<float>> data;
std::vector<float> labels;


void push_data(hjorth_params &H,int lab);
void train_SVM();
void test_SVM();
void save_model(CvSVM* mysvm);
void load_model(CvSVM* mySVM);
void clean_sig();
};
