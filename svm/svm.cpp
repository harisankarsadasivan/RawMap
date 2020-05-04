#include "svm.h"
void mySVM::train_SVM(){

randomize_samples(samples, labels);
const double max_nu = maximum_nu(labels);
svm_nu_trainer<kernel_type> trainer;
cout<<" doing cross validation"<<endl;
for(double gamma=0.0001;gamma<=1;gamma*=5)
	for(double nu=0.00001;nu<max_nu;nu*=5){
		trainer.set_kernel(kernel_type(gamma));
		trainer.set_nu(nu);
		cout<<"gamma:"<<gamma<<"nu:"<<nu<<endl;
		cout<<"cross validation accuracy"<<cross_validate_trainer(trainer,samples,labels,3);
	}
trainer.set_kernel(kernel_type(0.15625));
trainer.set_nu(0.15625);
typedef decision_function<kernel_type> dec_funct_type;
typedef normalized_function<dec_funct_type> funct_type;

funct_type learned_function;
//learned_function.normalizer = normalizer;  // save normalization information
learned_function.function = trainer.train(samples, labels); // perform the actual SVM training and save the results
cout << "\nnumber of support vectors in our learned_function is " << learned_function.function.basis_vectors.size() << endl;
serialize("saved_function.dat")<<learned_function;
    
}
void mySVM::push_data(hjorth_params &H, int lab){
sample_type samp;
samp(0)=H.A;
samp(1)=H.M;
samp(2)=H.C;

samples.push_back(samp);
labels.push_back(lab);
}
