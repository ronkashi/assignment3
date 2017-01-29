/*
 * main.cpp
 *
 *  Created on: 20.1.17
 *      Author: ron kashi
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "sp_image_proc_util.h"
#include "main_aux.h"

int main() {

	char path[MAX_STR_LEN], prefix[MAX_STR_LEN], suffix[MAX_STR_LEN];
	char fullPath[MAX_STR_LEN], queryPath[MAX_STR_LEN];
	int imgNum = 1, binNum = 0, featuresNum = 0, *nFeaturesPerImage;
	bool flag = true;
	SPPoint*** dataBaseHist;
	SPPoint*** dataBaseFeatures;
	//get data from user level
	if (SP_EXIT
			== spGetUserInput(path, prefix, suffix, &imgNum, &binNum,
					&featuresNum)) {
		return 0;
	}
	//the data base building level
	nFeaturesPerImage = (int*) malloc(imgNum * sizeof(int));
	if (NULL == nFeaturesPerImage) {
		printf(ALLOC_FAIL);
		return 0;
	}
	dataBaseHist = (SPPoint***) calloc(imgNum, sizeof(*dataBaseHist));
	if (NULL == dataBaseHist) {
		free(nFeaturesPerImage);
		printf(ALLOC_FAIL);
		return 0;
	}
	dataBaseFeatures = (SPPoint***) calloc(imgNum, sizeof(*dataBaseFeatures));
	if (NULL == dataBaseFeatures) {
		free(dataBaseHist);
		free(nFeaturesPerImage);
		printf(ALLOC_FAIL);
		return 0;
	}
	spMakeFullPath(fullPath, path, prefix, suffix);
	//calc data base of histogram distances
	if (SP_SUCCESS != spCalcHist(binNum, dataBaseHist, fullPath, imgNum)) {
		free(dataBaseFeatures);
		free(nFeaturesPerImage);
	}
	//calc data base of SIFT distances
	if (SP_SUCCESS
			!= spCalcSift(featuresNum, dataBaseFeatures, fullPath, imgNum,
					nFeaturesPerImage)) {
		spDestroyDBhist(dataBaseHist, imgNum);
		free(nFeaturesPerImage);
	}

	//the query image level (loop until the user input == "#")
	while ((SP_EXIT != spEnterQueryImg(queryPath)) && flag) {
		if (SP_SUCCESS
				!= spReturnGlobalSearch(queryPath, &imgNum, dataBaseHist,
						binNum)) {
			flag = false;
		}
		if (flag) {
			if (SP_SUCCESS
					!= spReturnLocalSearch(queryPath, &featuresNum,
							dataBaseFeatures, imgNum, nFeaturesPerImage)) {
				flag = false;
			}
		}
	}
	//free resource level
	spDestroyDBsift(dataBaseFeatures, imgNum, nFeaturesPerImage);
	spDestroyDBhist(dataBaseHist, imgNum);
	free(nFeaturesPerImage);
	return 0;

}
