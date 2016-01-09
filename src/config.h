/*
 * config.h
 *
 *  Created on: 19/7/2015
 *      Author: José Antonio Tula Leyva
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <stdio.h>

class Config {

private:

public:
	Config();
	Config(const char * file);
	~Config();

	char images_path[256];
	char features[256];
	char saida_path[256];
	char saida2_path[256];
	char svm_model[256];
	int hog_wsize;
	int hog_hsize;
	int resize;
	int resize_threshold;

	int hit_threshold;
	double scale;
	int group_threshold;


	FILE * openFileForReading(const char * fileName, const char * fallbackFileName);
	FILE * openFileForReading(const char * fileName);
	void closeFile(FILE * in);
	void fscanfError();

	int readInt(FILE * in);
	double readDouble(FILE * in);
	void readString(FILE * in, char * s);

};

#endif /* CONFIG_H_ */
