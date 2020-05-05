#include "hjorth_params.h"
#include<iostream>
#define CHUNK 6000
#define HEAD 2000
#define TAIL 1000
#define ENDBP HEAD+CHUNK
#define MIN_LEN CHUNK+HEAD+TAIL

void hjorth_params::calc_hjorth(raw_table &rt)
{
        std::vector<float> Y,dY;
        for(std::uint32_t i=HEAD;i<ENDBP;i++){
         Y.push_back(rt.raw[i]);
        }
        float var_Y,mob_Y;
        var_Y=hjorth_params::variance(Y);
        dY=hjorth_params::diff(Y);
        mob_Y=hjorth_params::mobility(dY,var_Y);
        C=hjorth_params::complexity(dY,mob_Y);
        A=var_Y;
        M=mob_Y;
}


//difference
std::vector<float> hjorth_params::diff(std::vector<float> &Y) 
{
std::vector<float> D;

for(std::uint32_t i=0;i<Y.size()-1;i++)
  D.push_back(Y[i+1]-Y[i]);
  return D;

}

//variance
float hjorth_params::variance(std::vector<float> &Y){
float M=0,var=0;
std::uint16_t N= Y.size();
for(std::uint32_t i=0;i<N;i++)
        M+=Y[i];
M=M/N;

for(std::uint32_t i=0;i<N;i++)
        var+=pow((Y[i]-M),2);
var=var/(N-1);

return var;
}


//mobility
float hjorth_params::mobility(std::vector<float> &dY,float &var_Y)
{

return pow(hjorth_params::variance(dY)/var_Y,0.5);

}

//complexity
float hjorth_params::complexity(std::vector<float> &dY,float &mob_Y){

std::vector<float> ddY=hjorth_params::diff(dY);
float var_dY=hjorth_params::variance(dY);
return hjorth_params::mobility(ddY,var_dY)/mob_Y;
}

