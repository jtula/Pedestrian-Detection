/*
 * localiza.h
 *
 *  Created on: 23/8/2015
 *      Author: José Antonio Tula Leyva
 */

#ifndef LOCALIZA_H_
#define LOCALIZA_H_

#include "config.h"
#include "treinaSVM.h"

class Locate{

private:
	Config conf;
	//test images qx??.ppm
	std::vector<std::string> testImages;

public:
	Locate(Config config);
	~Locate();

	void loadDataForTest();
	void locatePedestrian();
	void showDetections(cv::Mat & image, const std::vector<cv::Rect> &locations, const cv::Scalar &color);
};

#endif /* LOCALIZA_H_ */
