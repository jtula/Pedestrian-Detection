/*
 * treina.h
 *
 *  Created on: 23/8/2015
 *      Author: José Antonio Tula Leyva
 */

#ifndef TRAIN_H_
#define TRAIN_H_

#include "config.h"
#include <fstream>

#include "treinaSVM.h"

/*
 *  Class to train the SVM classifier
 */
class Train{

private:

	//positive images ap??.ppm
	std::vector<std::string> posImages;

	//negatives images ap??.ppm
	std::vector<std::string> negImages;

	cv::HOGDescriptor hog;

	//descriptors vector
	std::vector<cv::Mat> descriptorsVector;

	Config conf;
	int positives, negatives;
	void getSamples();
	void computeFeatures();
	void calculateFeaturesFromInput(const std::string& imageFilename, std::vector<float>& featureVector, cv::HOGDescriptor& hog);
	void saveDescriptors(std::fstream &criterion, bool type, std::vector<float> &descriptors);
	void trainWithDescriptors();
	void prepareData();

public:

	Train(Config config);
	~Train();
};

#endif /* TRAIN_H_ */
