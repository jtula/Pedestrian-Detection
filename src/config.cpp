/*
 * config.cpp
 *
 *  Created on: 19/7/2015
 *      Author: José Antonio Tula Leyva
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

using namespace std;

Config::Config(){

};

Config::Config(const char * file){

	char label[256];
	printf( "------\nConfiguration file: %s\n", file);
	FILE * in = openFileForReading(file);

	while( fscanf(in, "%s", label) == 1 ){

		if(strcmp(label, "images_path") == 0){

			readString(in, images_path);
		}
		else if(strcmp(label, "features") == 0){

			readString(in, features);
		}
		else if(strcmp(label, "svm_model") == 0){

			readString(in, svm_model);
		}
		else if(strcmp(label, "saida_path") == 0){

			readString(in, saida_path);
		}
		else if(strcmp(label, "saida2_path") == 0){

			readString(in, saida2_path);
		}
		else if(strcmp(label, "hog_wsize") == 0){

			hog_wsize = readInt(in);
		}

		else if(strcmp(label, "hog_hsize") == 0){

			hog_hsize = readInt(in);
		}
		else if(strcmp(label, "hit_threshold") == 0){

			hit_threshold = readInt(in);
		}
		else if(strcmp(label, "scale") == 0){

			scale = readDouble(in);
		}

		else if(strcmp(label, "group_threshold") == 0){

			group_threshold = readInt(in);
		}
		else if(strcmp(label, "resize") == 0){

			resize = readInt(in);
		}
		else if(strcmp(label, "resize_threshold") == 0){

			resize_threshold = readInt(in);
		}

	}
	closeFile(in);
}

Config::~Config(){}

FILE * Config::openFileForReading(const char * fileName, const char * fallbackFileName){

	FILE * in;
	in = fopen(fileName, "r");

	if(!in){

		printf("File: '%s' not found!", fileName);

		if(fallbackFileName){

			printf(" Opening '%s' instead...\n", fallbackFileName);
			in = fopen(fallbackFileName, "r");
		}
		else{

			printf(" No fallback file provided. Exiting...\n");
			exit(1);
		}
	}

	if(!in){

		printf("Fallback file '%s' not found! Exiting...\n", fallbackFileName);
		exit(1);
	}

	return in;
}

FILE * Config::openFileForReading(const char * fileName){

	return openFileForReading(fileName, NULL);
}

void Config::closeFile(FILE * in){

	fclose(in);
}

void Config::fscanfError(){

	printf("Error in fscanf!\n");
	exit(1);
}

int Config::readInt(FILE * in){

	int i;
	if(fscanf(in, "%d", &i) != 1) fscanfError();
	return i;
}

double Config::readDouble(FILE * in){

	double d;

	if(fscanf(in, "%lf", &d) != 1) fscanfError();
	return d;
}

void Config::readString(FILE * in, char * s){

	if(fscanf(in, "%s", s) != 1) {

		printf("Error in fscanf!\n");
		exit(1);
	}
}
