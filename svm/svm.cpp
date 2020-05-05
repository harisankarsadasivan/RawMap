#include "svm.h"
#ifndef SIMPLE_OPT
#include <dlib/global_optimization.h>
#endif
void mySVM::train_SVM(){

randomize_samples(samples, labels);


//simple optimization
#ifdef SIMPLE_OPT
const double max_nu = maximum_nu(labels);
svm_nu_trainer<kernel_type> trainer;
cout<<" doing cross validation"<<endl;
for(double gamma=0.0001;gamma<=1;gamma*=5)
	for(double nu=0.00001;nu<max_nu;nu*=5){
		trainer.set_kernel(kernel_type(gamma));
		trainer.set_nu(nu);
		cout<<"gamma:"<<gamma<<"nu:"<<nu<<endl;
		cout<<"cross validation: TP and TN rates: "<<cross_validate_trainer(trainer,samples,labels,3);
	}

//better otpimization[DEFAULT]
#else
auto cross_validation_score = [&](const double gamma, const double c1, const double c2) 
    {
typedef radial_basis_kernel<sample_type> kernel_type;
        svm_c_trainer<kernel_type> trainer;
        trainer.set_kernel(kernel_type(gamma));
        trainer.set_c_class1(c1);
        trainer.set_c_class2(c2);
matrix<double> result = cross_validate_trainer(trainer, samples, labels, 10);
        cout << "gamma: " << setw(11) << gamma << "  c1: " << setw(11) << c1 <<  "  c2: " << setw(11) << c2 <<  "  cross validation accuracy: " << result;
return mean(result);
    };
auto result = find_max_global(cross_validation_score, 
                                  {1e-5, 1e-5, 1e-5},  // lower bound constraints on gamma, c1, and c2, respectively
                                  {100,  1e6,  1e6},   // upper bound constraints on gamma, c1, and c2, respectively
                                  max_function_calls(50));

    double best_gamma = result.x(0);
    double best_c1    = result.x(1);
    double best_c2    = result.x(2);

    cout << " best cross-validation score: " << result.y << endl;
    cout << " best gamma: " << best_gamma << "   best c1: " << best_c1 << "    best c2: "<< best_c2  << endl;
#endif











//HARItrainer.set_kernel(kernel_type(best_gamma));
//HARItrainer.set_nu(0.15625);
//HARItypedef decision_function<kernel_type> dec_funct_type;
//HARItypedef normalized_function<dec_funct_type> funct_type;
//HARI
//HARIfunct_type learned_function;
//HARI//learned_function.normalizer = normalizer;  // save normalization information
//HARIlearned_function.function = trainer.train(samples, labels); // perform the actual SVM training and save the results
//HARIcout << "\nnumber of support vectors in our learned_function is " << learned_function.function.basis_vectors.size() << endl;
//HARIserialize("saved_function.dat")<<learned_function;
    
}
void mySVM::push_data(hjorth_params &H, int lab){
sample_type samp;
samp(0)=H.A;
samp(1)=H.M;
samp(2)=H.C;

samples.push_back(samp);
labels.push_back(lab);
}
