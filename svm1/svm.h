#pragma once
#include <iostream>
#include <dlib/svm.h>
#include "../fast5/hjorth_params.h"

using namespace std;
using namespace dlib;

typedef matrix<double, 3, 1> sample_type;
typedef radial_basis_kernel<sample_type> kernel_type;

class mySVM{
public:

std::vector<sample_type> samples;
std::vector<double> labels;

void push_data(hjorth_params &H,int lab);
void train_SVM();
float test_SVM();
};
