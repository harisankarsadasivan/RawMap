#pragma once
#include <iostream>
#include "nanopolish_fast5_loader.h"
class hjorth_params{
public:

std::vector<float> features;
//float A,M,C;
bool valid_data= true;
void calc_hjorth(raw_table &rt);
float variance(std::vector<float> &x);
std::vector<float> diff(std::vector<float> &Y);
float mobility(float &var_Y, float &var_dY);
float complexity(std::vector<float> &dY,float &mob_Y, float &var_dY);
void clean_sig(std::vector<float> &Y, std::vector<float> &Z);
};
