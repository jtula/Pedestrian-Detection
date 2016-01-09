/*
 * treina.cpp
 *
 *  Created on: 23/8/2015
 *      Author: Jose A Tula Leyva
 */
#include <ios>
#include <stdio.h>
#include <dirent.h>
#include "treina.h"
#include <ctime>
#include <sys/timeb.h>

/*!
  Construtor
 */
Train::Train(Config config){

	clock_t start, finish;
	start = clock();
	this->conf = config;
	hog.winSize = cv::Size(config.hog_wsize, config.hog_hsize);
	positives = 0;
	negatives = 0;
	getSamples();
	computeFeatures();
	//prepareData();
	trainWithDescriptors();
	finish = clock();
	std::cout << "Tempo utilizado (segundos): "<< (finish - start)/CLOCKS_PER_SEC << std::endl;
}

/*!
  Destrutor
 */
Train::~Train(){
}


/*
 * Loads in posImages and negImages vectors
 * the positive and negative images of the images folder.
 */
void Train::getSamples(){

	std::string root = conf.images_path;
    DIR* images_path = opendir(root.c_str());

    if( images_path != NULL ){

        struct dirent* dent;

        while( (dent = readdir(images_path))){

			size_t foundAp = std::string(dent->d_name).find("ap");
			size_t foundAn = std::string(dent->d_name).find("an");

			if(foundAp!=std::string::npos){

				posImages.push_back(std::string(dent->d_name));
				positives++;
			}
 			else if(foundAn!=std::string::npos){

 				negImages.push_back(std::string(dent->d_name));
 				negatives++;
			}
        }

        closedir(images_path);
   }
}

/*
 * Calculate HOG for each sample and save descriptors
 */
void Train::computeFeatures(){

	cv::Mat image, gray;
	cv::Size sizeHOG(8,8);
	cv::Size padding(0,0);
	std::vector<float> descriptors;
	std::vector<cv::Point> locations;
	std::string path = conf.images_path;
	std::fstream criterion;
	criterion.open(conf.features);
	int cont = 0;

	if(criterion.is_open()){

		for (unsigned i = 0; i < posImages.size(); ++i) {

			image = cv::imread(conf.images_path+posImages[i],1);
			//cv::cvtColor(image, gray, CV_BGR2GRAY);
			if (image.cols != conf.hog_wsize || image.rows != conf.hog_hsize) {
				std::cout<<"fazendo resize da imagen "<< posImages[i] << "...." <<std::endl;
				cv::Size size(conf.hog_wsize,conf.hog_hsize);
				cv::resize(image, image, size);
			}

			hog.compute(image, descriptors, sizeHOG, padding, locations);
			descriptorsVector.push_back(cv::Mat(descriptors).clone());
			saveDescriptors(criterion, true, descriptors);
			locations.clear();
			image.release();
		}

		for (unsigned int i = 0; i< negImages.size(); ++i) {

			image = cv::imread(conf.images_path+negImages[i],1);
			//cv::cvtColor(image, gray, CV_BGR2GRAY);
			if (image.cols != conf.hog_wsize || image.rows != conf.hog_hsize) {
				std::cout<<"fazendo resize da imagen "<< negImages[i] << "...." <<std::endl;
				cv::Size size(conf.hog_wsize,conf.hog_hsize);
				cv::resize(image, image, size);
			}

			hog.compute(image, descriptors, sizeHOG, padding, locations);
			descriptorsVector.push_back(cv::Mat(descriptors).clone());
			saveDescriptors(criterion, true, descriptors);
			locations.clear();
			image.release();
		}

		criterion.flush();
		criterion.close();
	}
	else{

		std::cout<<"problema para abrir o arquivo" <<std::endl;
	}

}

/*
void Train::prepareData(){

	FILE *criterion;
	long ll, max_words_doc, max_docs;
	char *line,*comment;
	char c;
	vector<float> descriptors;
	svmlight::nol_ll(conf.features,&max_docs,&max_words_doc,&ll);
	line = (char *)calc_malloc(sizeof(char)*ll);

	if ((criterion = fopen (conf.features, "r")) == NULL){

		perror (conf.features); exit (1);
	}

	int cont = 0;
	string number = "";

	while((!feof(criterion)) && fgets(line,(int)ll,criterion)){

		string iterLine(line);

		for (int i = 0; i < iterLine.size(); ++i) {

			c = iterLine[i];
			if(c=='+' || c=='-')
				continue;
			if (!isspace(c)){
				number+=c;
			}
			else if(number.size()>0){
				string::size_type sz;
				double n = std::stod(number,&sz);
				descriptors.push_back(n);
				number = "";
			}
		}

		descriptorsVector.push_back(cv::Mat(descriptors).clone());
		descriptors.clear();
	}

}
*/


/*
 * Salva os descritores num arquivo(descritores.txt)
 *
 *
 */
void Train::saveDescriptors(std::fstream &criterion, bool type, std::vector<float> &descriptors){

	if(!descriptors.empty()){

		if(type){

			criterion << "+";
		}
		else{

			criterion << "-";
		}

		for(unsigned int i = 0; i < descriptors.size(); ++i){

			criterion << " " << i <<descriptors[i];
		}

		criterion << std::endl;
		descriptors.clear();
	}
}


/*
 *	Function to convert the training data to be used by the SVM classifier
 */
void convert_to_ml(const std::vector<cv::Mat> &train_samples, cv::Mat& trainData )
{
    const int rows = (int)train_samples.size();
    const int cols = (int)std::max( train_samples[0].cols, train_samples[0].rows );

    cv::Mat tmp(1, cols,CV_32FC1);
    trainData = cv::Mat(rows,cols,CV_32FC1 );
    std::vector<cv::Mat>::const_iterator itr = train_samples.begin();
    std::vector<cv::Mat>::const_iterator end = train_samples.end();

    for( int i = 0 ; itr != end ; ++itr, ++i ) {

        CV_Assert( itr->cols == 1 || itr->rows == 1 );

        if( itr->cols == 1 ) {

            transpose( *(itr), tmp );
            tmp.copyTo( trainData.row( i ) );
        }
        else if( itr->rows == 1 ) {

            itr->copyTo( trainData.row( i ) );
        }
    }
}


/*
 * Train the SVM classifier with the descriptors obtained in computeFeatures()
 * and saves the file criterio.xml the criteria of decision.
 */
void Train::trainWithDescriptors(){

    cv::Mat train_data;
    int samples = positives + negatives;
    convert_to_ml(descriptorsVector, train_data );
    cv::Mat labels(samples, 1, CV_32SC1);
    labels.rowRange(0, positives).setTo(1);
    labels.rowRange(positives, samples).setTo(-1);

    //default parameters used in the original article
    cv::SVMParams params;
        params.coef0 = 0.0;
        params.degree = 3;
        params.term_crit.epsilon = 1e-3;
        params.gamma = 0;
        params.kernel_type = cv::SVM::LINEAR;
        params.nu = 0.5;
        params.p = 0.1;
        params.C = 0.01;
        params.svm_type = cv::SVM::C_SVC;
	TrainSVM svm;
	std::cout<<"treinando o classificador SVM..."<<std::endl;
    svm.train(train_data, labels, cv::Mat(), cv::Mat(), params);
    svm.save(conf.svm_model);
    std::cout<<"criterios do classificador SVM salvos."<<std::endl;
}
