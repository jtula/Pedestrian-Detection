/*
 * trainSVM.h
 *
 *  Created on: 5/9/2015
 *      Author: José Antonio Tula Leyva
 */

#ifndef SRC_TREINASVM_H_
#define SRC_TREINASVM_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include <iostream>

/*
 *	Derived class from CvSVM class to get a decision function
 *	because it's protected
 */
class TrainSVM: public cv::SVM{

public:
	void getSVMVector(std::vector<float>& hog_vector) const;
};


#endif /* SRC_TREINASVM_H_ */
