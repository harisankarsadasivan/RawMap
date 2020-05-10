#include "mysvm.h"
void mySVM::save_model(CvSVM* mysvm){
	
	if(remove("model.data")!=0) puts("model file does not exist");
	else puts("Existing model file succesfully deleted");
	CvFileStorage* fs = cvOpenFileStorage("model.data", 0, CV_STORAGE_WRITE);
	if(!fs)cerr<<"Error writing model to file\n";
	mysvm->write(fs,"mysvm");
	cvReleaseFileStorage(&fs);
	cout<<"Model file created"<<endl;
}
void mySVM::load_model(CvSVM* mysvm){

	CvFileStorage* fs = cvOpenFileStorage("model.data", 0, CV_STORAGE_READ);
	mysvm->read(fs,cvGetFileNodeByName(fs,0,"mysvm"));
	cvReleaseFileStorage(&fs);
}
void mySVM::test_SVM(){
	mySVM::data_size=mySVM::data.size();
        CvSVM* mysvm = new CvSVM;
 	mySVM::load_model(mysvm);

        Mat test_sample;
        int correct_class = 0;
        int wrong_class = 0;
        int false_positives [2] = {0,0};
        float result;


        Mat testing_data = Mat(mySVM::data_size, 3, CV_32FC1);
        Mat testing_classifications = Mat(mySVM::data_size, 1, CV_32FC1);

	for(std::uint32_t i=0;i<mySVM::data_size;i++){
		for(std::uint16_t j=0;j<3;j++){
			testing_data.at<float>(i,j) = mySVM::data[i][j];
		}
		testing_classifications.at<float>(i,0)= mySVM::labels[i];
	}
        for (std::uint32_t tsample = 0; tsample < mySVM::data_size; tsample++)
        {

            // extract a row from the testing matrix

            test_sample = testing_data.row(tsample);

            // run SVM classifier

            result = mysvm->predict(test_sample);

            printf("Testing Sample %i -> class result (digit %f)\n", tsample, result);

            // if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point implementation!)

            if (fabs(result - testing_classifications.at<float>(tsample, 0))
                    >= FLT_EPSILON)
            {
                // if they differ more than floating point error => wrong class

                wrong_class++;
                false_positives[(int) testing_classifications.at<float>(tsample, 0)]++;

            }
            else
            {

                // otherwise correct

                correct_class++;
            }
        }

        printf( "\n"
                "\tCorrect classification: %d (%g%%)\n"
                "\tWrong classifications: %d (%g%%)\n",
                
                correct_class, (double) correct_class*100/mySVM::data_size,
                wrong_class, (double) wrong_class*100/mySVM::data_size);

        for (int i = 0; i < 2; i++)
        {
            printf( "\tClass (digit %d) false postives 	%d (%g%%)\n", i,
                    false_positives[i],
                    (double) false_positives[i]*100/mySVM::data_size);
        }

 }
void mySVM::train_SVM(){
    mySVM::data_size=mySVM::data.size();
    printf ("OpenCV version %s (%d.%d.%d)\n",CV_VERSION,CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
	Mat training_data = Mat(mySVM::data_size, N, CV_32FC1);
 	Mat training_classifications = Mat(mySVM::data_size, 1, CV_32FC1);
	for(std::uint32_t i=0;i<mySVM::data_size;i++){
		for(std::uint16_t j=0;j<3;j++){
			training_data.at<float>(i,j) = mySVM::data[i][j];
		}
		training_classifications.at<float>(i,0)= mySVM::labels[i];
	}
        CvSVMParams params = CvSVMParams(
                                 CvSVM::C_SVC,   // Type of SVM, here N classes (see manual)
                                 CvSVM::RBF,  // kernel type (see manual)
                                 0.0,			// kernel parameter (degree) for poly kernel only
                                 0.33333,			// kernel parameter (gamma) for poly/rbf kernel only
                                 0.0,			// kernel parameter (coef0) for poly/sigmoid kernel only
                                 10,				// SVM optimization parameter C
                                 0,				// SVM optimization parameter nu (not used for N classe SVM)
                                 0,				// SVM optimization parameter p (not used for N classe SVM)
                                 NULL,			// class wieghts (or priors)
                                 // Optional weights, assigned to particular classes.
                                 // They are multiplied by C and thus affect the misclassification
                                 // penalty for different classes. The larger weight, the larger penalty
                                 // on misclassification of data from the corresponding class.

                                 // termination criteria for learning algorithm

                                 cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001)

                             );
	
        // train SVM classifier (using training data)

        CvSVM* mysvm = new CvSVM;

#if (USE_OPENCV_GRID_SEARCH_AUTOTRAIN)

        // train using auto training parameter grid search if it is available
        // N.B. this does not search kernel choice

        mysvm->train_auto(training_data, training_classifications, Mat(), Mat(), params, 10);
        params = mysvm->get_params();
        printf( "\nUsing optimal parameters degree %f, gamma %f, ceof0 %f\n\t C %f, nu %f, p %f\n",
                params.degree, params.gamma, params.coef0, params.C, params.nu, params.p);

#else

        // otherwise use regular training and use parameters manually specified above

        mysvm->train(training_data, training_classifications, Mat(), Mat(), params);

#endif

        // get the number of support vectors used to define the SVM decision boundary

        printf("Number of support vectors for trained SVM = %i\n", mysvm->get_support_vector_count());
	mySVM::save_model(mysvm);
}
void mySVM::push_data(hjorth_params &H, int lab){

data.push_back(H.features);
labels.push_back(lab);
}
