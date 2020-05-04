#pragma once
#include <iostream>
#include "nanopolish_fast5_loader.h"
class hjorth_params{
public:
float A,M,C;
void calc_hjorth(raw_table &rt);
float variance(std::vector<float> &Y);
std::vector<float> diff(std::vector<float> &Y);
float mobility(std::vector<float> &dY,float &var_Y);
float complexity(std::vector<float> &dY,float &mob_Y);

};
