#include "hjorth_params.h"
#include<iostream>
#include <algorithm>
#include <math.h>
#define CHUNK 6000
#define HEAD 2000
#define TAIL 1000
#define ENDBP HEAD+CHUNK
#define MIN_LEN CHUNK+HEAD+TAIL
void hjorth_params::clean_sig(std::vector<float> &Z, std::vector<float> &Y)
{
	size_t size=Z.size();
	float MED,MAD=0,low=0,hi=0;
        std::vector<float> Z1=Z;
	std::sort(Z1.begin(),Z1.end());
	if(size%2==0) MED=0.5*(Z1[size/2-1]+Z1[size/2]);
	else MED=Z1[0.5*size];

	Z1.clear();
	//std::cout<<"Original series:  ";
	for(std::uint16_t i=0;i<size;i++){
		//std::cout<<Z[i]<<",";
		Z1.push_back(fabs(Z[i]-MED));
	}
	
	std::sort(Z1.begin(),Z1.end());
	if(size%2==0) MAD=0.5*(Z1[size/2-1]+Z1[size/2]);
	else MAD=Z1[0.5*size];
	//std::cout<<"\nsorted deviations:  ";
	//for(std::uint16_t i=0;i<size;i++){
	//	//std::cout<<Z1[i]<<",";
	//}
	Z1.clear();
	//std::cout<<std::endl<<"median"<<MED<<std::endl;

	//std::cout<<"mad"<<MAD<<std::endl;
	low=MED-5*MAD; 
	hi=MED+5*MAD;
	std::uint16_t k=0;

	//std::cout<<"Clean series:  ";
	for(std::uint16_t i=HEAD;i<MIN_LEN;i++){
		if(Z[i]>= low && Z[i]<=hi)	{
				
			Y.push_back((Z[i]-MED)/MAD);
			//std::cout<<Y[k]<<",";
			k++;
		}
		if(k==CHUNK) break;
	}
	if(Y.size()!=CHUNK) {
	std::cerr<<"Error: High deviations in read/ Noisy read because clean chunk size is: "<<Y.size()<<"\n";
	hjorth_params::valid_data=false;
	}
	else hjorth_params::valid_data=true;
	//std::cout<<"\nfirst element"<<Y[0]<<std::endl;

	

}
void hjorth_params::calc_hjorth(raw_table &rt)
{
 	std::vector<float> Y,dY,Z;
        for(std::uint32_t i=0;i<ENDBP+TAIL;i++){
         Z.push_back(rt.raw[i]);
        }
	hjorth_params::clean_sig(Z,Y);
	Z.clear();
	
	

        float var_Y,mob_Y,var_dY;
        dY=hjorth_params::diff(Y);
 	//std::cout<<"size of signal:"<<Y.size()<<"\n";
	//std::cout<<"Before calculating var:"<<std::endl;
//        for(int i=0;i<Y.size();i++)
//		std::cout<<Y[i]<<",";
        var_Y=hjorth_params::variance(Y);
        var_dY=hjorth_params::variance(dY);
	
        mob_Y=hjorth_params::mobility(var_Y,var_dY);
        features.push_back(var_Y);
        features.push_back(mob_Y);

        features.push_back(hjorth_params::complexity(dY,mob_Y,var_dY));
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
float hjorth_params::variance(std::vector<float> &x){
float M=0,var=0;
std::uint16_t N= x.size();
for(std::uint32_t i=0;i<N;i++)
        M+=x[i];
M=M/N;

for(std::uint32_t i=0;i<N;i++)
        var+=pow((x[i]-M),2);
var=var/(N);
return var;
}


//mobility
float hjorth_params::mobility(float &var_Y, float &var_dY)
{

return pow(var_dY/var_Y,0.5);

}

//complexity
float hjorth_params::complexity(std::vector<float> &dY,float &mob_Y, float &var_dY){

std::vector<float> ddY=hjorth_params::diff(dY);
float var_ddY=hjorth_params::variance(ddY);
return hjorth_params::mobility(var_dY,var_ddY)/mob_Y;
}

