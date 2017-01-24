/*
 * main.cpp
 *
 *  Created on: 8 áéðå 2017
 *      Author: Arnit
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "sp_image_proc_util.h"
#include "main_aux.h"
using namespace cv;
using namespace std;


int main() {


	char path[MAX_STR_LEN];
	char prefix[MAX_STR_LEN];
	char suffix[MAX_STR_LEN];
	char fullPath[MAX_STR_LEN];
	char queryPath[MAX_STR_LEN];
	int imgNum=1, binNum=0, featuresNum=0, *nFeaturesPerImage;
	SPPoint*** dataBaseHist;
	SPPoint*** dataBaseFeatures;

	if(SP_EXIT==spGetUserInput(path,prefix,suffix, &imgNum,&binNum, &featuresNum)){
		return 0;
	}

	nFeaturesPerImage = (int*) malloc(imgNum * sizeof (int));

	dataBaseHist=(SPPoint***)calloc(imgNum,sizeof(*dataBaseHist));
	if(NULL == dataBaseHist){
		printf(ALLOC_FAIL);
		return 0;
	}
	dataBaseFeatures=(SPPoint***)calloc(imgNum,sizeof(*dataBaseFeatures));
	if(NULL == dataBaseFeatures){
		//TODO free dataBaseHist
		printf(ALLOC_FAIL);
		return 0;
	}

	spMakeFullPath(fullPath,path,prefix,suffix);

	spCalcHist(binNum, dataBaseHist,fullPath, imgNum);

	spCalcSift(featuresNum, dataBaseFeatures,fullPath, imgNum, nFeaturesPerImage);

	while(SP_EXIT!=spEnterQueryImg(queryPath)){
		spReturnGlobalSearch(queryPath,&imgNum,dataBaseHist,binNum);

		spReturnLocalSearch(queryPath, &featuresNum,dataBaseFeatures, imgNum, nFeaturesPerImage);

	}
	free(nFeaturesPerImage);
	//TODO free dataBaseHist
	//TODO free all
	return 0;

}
