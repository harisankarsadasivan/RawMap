#include "mysvm.h"
#include<stdio.h>
void mySVM::save_model(){
	
	if(remove("model.data")!=0) puts("model file does not exit");
	else puts("Existing model file succesfully deleted");
	svm_save_model("model.data",model);
	cout<<"Model file created"<<endl;
}
void mySVM::load_model(){
	model=svm_load_model("model.data");
}
void mySVM::test_SVM(){
 
 mySVM::load_model();

 struct svm_node *x,*x_space;
 float acc=0,predict_label=0;
 x= (struct svm_node *) malloc(N*sizeof(struct svm_node));
 //x = (struct svm_node *) realloc(x,N*sizeof(struct svm_node));
 //x_space = (struct svm_node *) realloc(x_space,(N+1)*sizeof(struct svm_node));
 x_space = Malloc(struct svm_node,N+1);
 for(std::uint16_t i=0;i<data.size();i++){
//	x_space = Malloc(struct svm_node, (N+1)*1);
	std::uint16_t j=0;

       
        x=&x_space[j];
        
        for(std::uint16_t k=0;k<data[i].size();++k,++j){
        	x[j].index=k+1;
        	x[j].value=data[i][j];
        	
        	
        }

 //cout<<"1"<<endl;
        x_space[j].index=-1;
        x_space[j].value=0;

 predict_label=svm_predict(model,x);

 //cout<<"2"<<endl<<labels[i];
 if(predict_label==labels[i])
	acc++;
 }
 cout<<"acc"<<acc<<"label size"<<labels.size()<<endl;
 cout<<"Test Accuracy is: "<<acc/labels.size()<<endl;
}
void mySVM::train_SVM(){

	uint16_t tsize=data.size();
	cout<<"Training size"<<data.size()<<endl;
	prob.l=tsize;
	prob.y=Malloc(double,prob.l);
	prob.x=Malloc(struct svm_node *,prob.l);
	x_space = Malloc(struct svm_node, (N+1)*prob.l);
	for(std::uint16_t i=0;i<prob.l;i++){
		prob.y[i]=labels[i];		
	}
	std::uint16_t j=0;
	
	for(std::uint16_t i=0;i<prob.l;i++){
		
		prob.x[i]=&x_space[j];
		
		for(std::uint16_t k=0;k<data[i].size();++k,++j){
			x_space[j].index=k+1;
			x_space[j].value=data[i][j];
			
			
		}
		x_space[j].index=-1;
		x_space[j].value=0;
		j++;

	}
	
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0.333333333333;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 4;
	param.eps = 0.001;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

        auto err=svm_check_parameter(&prob,&param);
	if(err==NULL)
	cout<<"Params are checked"<<endl;
	else
	cout<<err;
	model=svm_train(&prob,&param);
        save_model();
}
void mySVM::push_data(hjorth_params &H, int lab){

data.push_back(H.features);
labels.push_back(lab);
}
