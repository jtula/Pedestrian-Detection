/*
 * Created on  : Aug 23, 2015
 * Author      : Jose A Tula Leyva
 * Version     : 0.1
 * Copyright   : Free
*/

#include <iostream>
#include <chrono>
#include "config.h"
#include "localiza.h"
#include "treina.h"

void usage(char * cmd){

	printf("\nusage: %s <metodo>\n\n", cmd);
	printf("<metodo>   : \'t treina com imagens de mostra\' ,"
						"\'l\' localiza pedestres, \n");
	exit(1);
}

int main(int argc, char ** argv) {

	char path[10], *method;
	method = argv[1];

	if(argc != 2){

		usage(argv[0]);
	}

	if ( (strlen(method) != 1) || (method[0] != 't') && (method[0] != 'l') ){

		usage(argv[0]);
	}

	//configuration file
	Config conf("config.cfg");

	try {

		if(method[0]=='t'){
			//loads the test images and trains the classifier
			Train * train = new Train(conf);
		}
		else if(method[0]=='l'){
			//locate pedestrian in the test images
			Locate * locate = new Locate(conf);

		}

	}
	catch (cv::Exception e) {

		std::cout << "An exception has accurred: \n";
	}

	return(0);
}
