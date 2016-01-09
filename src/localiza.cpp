/*
 * localiza.cpp
 *
 *  Created on: 23/8/2015
 *      Author: José Antonio Tula Leyva
 */
#include "localiza.h"
#include <ios>
#include <stdio.h>
#include <dirent.h>
#include <iostream>

using namespace std;

Locate::Locate(Config config){

	clock_t start, finish;
	start = clock();
	this->conf = config;
	std::cout<<"Carregando imagens para localizar os pedrestres..... "<<std::endl;
	loadDataForTest();
	locatePedestrian();
	finish = clock();
	std::cout << "Tempo utilizado (segundos): "<< (finish - start)/CLOCKS_PER_SEC << std::endl;
}

/*
 * Load the test images saved in the folder images/??qx.ppm
 */
void Locate::loadDataForTest(){

	string root = conf.images_path;
    DIR* images_path = opendir(root.c_str());

    if( images_path != NULL ){

        struct dirent* dent;

        while( (dent = readdir(images_path))){

			size_t foundQx = string(dent->d_name).find("qx");

			if(foundQx!=string::npos){

				testImages.push_back(string(dent->d_name));
				std::cout<<"imagem "<< string(dent->d_name) << " carregada"<<std::endl;
			}
        }

        closedir(images_path);
   }
   std::sort(testImages.begin(), testImages.end());
   std::cout<< "......" << testImages.size() <<" imagens carregadas."<<std::endl;
}

/*
 * Function to paint the rectangles with detected pedestrians
 *
 * @param image - image
 * @param locations - Pedestrian coordinates detected
 * @param color - the color to paint the rectangles
 */
void Locate::showDetections(cv::Mat & image, const std::vector<cv::Rect> &locations, const cv::Scalar &color) {

	if( !locations.empty() ) {

	        vector< cv::Rect >::const_iterator loc = locations.begin();
	        vector< cv::Rect >::const_iterator end = locations.end();

	        for( ; loc != end ; ++loc ) {

	            cv::rectangle( image, *loc, color, 2 );
	        }
	    }
}

/*!
 * Function to locate pedestrians using the saved criteria
 */
void Locate::locatePedestrian(){

	TrainSVM svm;
    svm.load(conf.svm_model);

    cv::HOGDescriptor my_hog, opencv_hog;
    cv::Scalar green_color( 0, 255, 0 );
    cv::Scalar red_color( 0, 0, 255 );

    int negatives=0, positives=0;
    cv::Size win_stride(8,8);
	cv::Size padding(32,32);
	std::vector<float> foundDetection;
	std::vector<cv::Point> locations;

	opencv_hog.setSVMDetector(opencv_hog.getDefaultPeopleDetector());
    cv::Mat img_copy;

    std::vector<float> hog_detector;
    svm.getSVMVector(hog_detector);

    cv::HOGDescriptor hog;
    hog.winSize = cv::Size(conf.hog_wsize, conf.hog_hsize);
    hog.setSVMDetector(hog_detector);

    std::vector<cv::Rect> coords;
    std::vector<cv::Rect> coords_hogopencv;

    for (int i = 0; i < testImages.size(); ++i) {

    	cv::Mat image = cv::imread(conf.images_path+testImages[i], 1);

    	if((image.cols- conf.hog_wsize) < conf.resize_threshold || (image.rows - conf.hog_hsize) < conf.resize_threshold){
    		cv::Size size( (int)(image.cols+conf.resize), (int)(image.rows+conf.resize) );
    		cv::resize( image, image,  size);
    		std::cout<<"fazendo resize da imagen "<< testImages[i] << "...." <<std::endl;
    	}

    	std::cout<<(conf.images_path+testImages[i])<<std::endl;
    	locations.clear();
    	hog.detectMultiScale(image, coords, conf.hit_threshold, win_stride, padding, 1.05, conf.group_threshold);
    	opencv_hog.detectMultiScale(image, coords_hogopencv, conf.hit_threshold, win_stride, padding, 1.05, conf.group_threshold);
    	img_copy = image.clone();
    	showDetections(img_copy, coords, green_color);
    	showDetections(img_copy, coords_hogopencv, red_color);
    	imwrite(conf.saida2_path+testImages[i], img_copy);
	}

    for (int i = 0; i < testImages.size(); ++i) {

    	cv::Mat image = cv::imread(conf.images_path+testImages[i], 1);

    	if((image.cols- conf.hog_wsize) < conf.resize_threshold || (image.rows - conf.hog_hsize) < conf.resize_threshold){
    		cv::Size size( (int)(image.cols+conf.resize), (int)(image.rows+conf.resize) );
    		cv::resize( image, image,  size);
    		std::cout<<"fazendo resize da imagen "<< testImages[i] << "...." <<std::endl;
    	}

    	locations.clear();
    	hog.detectMultiScale(image, coords, conf.hit_threshold, win_stride, padding, 1.05, conf.group_threshold);
    	img_copy = image.clone();
    	showDetections(img_copy, coords, green_color);
    	imwrite(conf.saida_path+testImages[i], img_copy);
	}
}
